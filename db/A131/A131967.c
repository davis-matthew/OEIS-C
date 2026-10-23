/*
 * A131967  Farey fractal sequence.
 * https://oeis.org/A131967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of labels assigned to reduced fractions (a,b) in order
 *            of their first appearance in successive Farey sequences
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the n-th term (1‑based) of OEIS sequence
   A131967 – the Farey fractal sequence.  It builds the sequence incrementally
   on the first call, storing the result in static storage so that subsequent
   calls (in any order) are answered in O(1).  The algorithm generates
   Farey fractions of increasing order, assigns a new integer label to each
   fraction on its first appearance, and outputs the labels in the order
   the fractions appear in each Farey sequence.  The generated labels start
   at 1 and increase by 1 each time a new fraction is encountered.  The
   resulting list of labels is exactly the sequence A131967.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*--------- Internal data structures -------------------------------------*/

typedef struct {
    uint64_t key;   /* key = (b<<32) | a */
    seq_t    value;
    bool     occupied;   /* true if the slot is used */
} HashEntry;

/* Hash table -----------------------------------------------------------*/
static HashEntry *hash_table   = NULL;
static size_t    hash_size     = 0;      /* power of two allocation size */
static size_t    hash_count    = 0;      /* number of occupied entries */

/* Sequence storage -----------------------------------------------------*/
static seq_t  *seq_array     = NULL;
static size_t seq_capacity    = 0;      /* capacity of seq_array */
static size_t seq_length      = 0;      /* number of stored terms */

/* Current order processed ---------------------------------------------*/
static idx_t current_order    = 0;
static seq_t next_label       = 0;       /* value of the next label to give */

/*---------------------------------------------------------------------*/
/* Simple Euclidean gcd for idx_t                                    */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Simple hash: uses upper 32 bits of key as part of hash but works for
   small keys; the table size is always a power of two so mask is trivial. */
static inline size_t hash_mask(void)
{
    return hash_size - 1;
}

static inline size_t hash_uint64(uint64_t key)
{
    return (size_t)((key) ^ (key >> 32) ^ (key >> 16) ^ (key >> 8) ^
                    (key >> 4)) & hash_mask();
}

/* Initialize hash table with given power of two size. */
static void init_hash_table(size_t min_size)
{
    /* Choose power of two >= min_size * 2 to keep load factor < 0.5 */
    size_t sz = 2;
    while (sz < min_size * 2) sz <<= 1;
    hash_size = sz;
    hash_table = (HashEntry *)calloc(hash_size, sizeof(HashEntry));
    hash_count = 0;
}

/* Find value for key; returns true if found and sets *out_value. */
static bool hash_find(uint64_t key, seq_t *out_value)
{
    size_t idx = hash_uint64(key);
    for (;;) {
        if (!hash_table[idx].occupied) return false;
        if (hash_table[idx].key == key) {
            *out_value = hash_table[idx].value;
            return true;
        }
        idx = (idx + 1) & hash_mask();
    }
}

/* Insert new key/value; assumes key does not exist yet. */
static void hash_insert(uint64_t key, seq_t value)
{
    size_t idx = hash_uint64(key);
    for (;;) {
        if (!hash_table[idx].occupied) {
            hash_table[idx].occupied = true;
            hash_table[idx].key = key;
            hash_table[idx].value = value;
            hash_count++;
            return;
        }
        idx = (idx + 1) & hash_mask();
    }
}

/* Add a term to the sequence array. */
static void seq_push_back(seq_t val)
{
    if (seq_length == seq_capacity) {
        size_t new_cap = (seq_capacity == 0) ? 16 : seq_capacity * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
        seq_array = new_arr;
        seq_capacity = new_cap;
    }
    seq_array[seq_length++] = val;
}

/* Fraction representation for sorting. */
typedef struct { idx_t a; idx_t b; } Fraction;

/* Comparator for qsort: compare a/b < c/d  */
static int frac_compare(const void *p1, const void *p2)
{
    const Fraction *f1 = (const Fraction *)p1;
    const Fraction *f2 = (const Fraction *)p2;
    /* cross multiply using 128 bits to avoid overflow */
    __int128 left  = (__int128)f1->a * f2->b;
    __int128 right = (__int128)f2->a * f1->b;
    if (left < right) return -1;
    if (left > right) return 1;
    return 0;
}

/* Generate one Farey order: produce all reduced a/b with 0<=a<=b<=order
   and push the corresponding labels into seq_array.  The labels are
   looked up in the hash table; new fractions obtain a fresh label. */
static void generate_order(idx_t order)
{
    /* dynamic array for fractions of this order */
    size_t fracs_cap = 16;
    size_t fracs_len = 0;
    Fraction *fracs = (Fraction *)malloc(fracs_cap * sizeof(Fraction));

    /* Build list of fractions for this order */
    for (idx_t b = 1; b <= order; ++b) {
        if (b == 1) {
            /* 0/1 and 1/1 are always present when b == 1 */
            if (fracs_len == fracs_cap) {
                fracs_cap <<= 1;
                fracs = (Fraction *)realloc(fracs, fracs_cap * sizeof(Fraction));
            }
            fracs[fracs_len++] = (Fraction){0, 1};
            fracs[fracs_len++] = (Fraction){1, 1};
            continue;
        }
        for (idx_t a = 1; a < b; ++a) {
            if (gcd_idx(a, b) != 1) continue;   /* skip non–reduced */
            if (fracs_len == fracs_cap) {
                fracs_cap <<= 1;
                fracs = (Fraction *)realloc(fracs, fracs_cap * sizeof(Fraction));
            }
            fracs[fracs_len++] = (Fraction){a, b};
        }
    }

    /* Sort fractions by ascending value a/b */
    qsort(fracs, fracs_len, sizeof(Fraction), frac_compare);

    /* Emit labels for sorted fractions */
    for (size_t i = 0; i < fracs_len; ++i) {
        idx_t a = fracs[i].a;
        idx_t b = fracs[i].b;
        uint64_t key = ((uint64_t)b << 32) | (uint64_t)a;
        seq_t lbl;
        if (!hash_find(key, &lbl)) {
            lbl = ++next_label;           /* new fraction -> new label */
            hash_insert(key, lbl);
        }
        seq_push_back(lbl);
    }

    free(fracs);
}

/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Initialise static state on first call */
    static bool initialised = false;
    if (!initialised) {
        initialised = true;
        hash_size     = 0;
        hash_table    = NULL;
        seq_array     = NULL;
        seq_capacity  = 0;
        seq_length    = 0;
        current_order = 0;
        next_label    = 0;
        /* Allocate hash table with a reasonable starting size. */
        init_hash_table(1024);
    }

    /* Generate orders until we have at least n terms. */
    while (seq_length < n) {
        ++current_order;
        generate_order(current_order);
    }

    return seq_array[n - 1];
}
