/*
 * A180076  a(n+1) = if it exists, smallest number not occurring earlier that
 *          is contained in binary representation of a(n), otherwise: a(n+1) =
 *          3*a(n)+1; a(0) = 0.
 * https://oeis.org/A180076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 0 For n ≥ 1: a(n) = smallest number not occurring earlier
 *            that is contained in the binary representation of a(n−1),
 *            otherwise a(n) = 3·a(n−1) + 1
 * Range:     u8 n <= 69, u16 n <= 7579, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------- hash table for seen numbers ----------------- */

/* hash table entry */
typedef struct {
    seq_t key;
    bool used;
} hentry_t;

static hentry_t *hash_table = NULL;
static size_t  hash_cap   = 0;   /* power of two */
static size_t  hash_count = 0;

/* forward declaration */
static void hash_expand(void);

/* insert a value into the hash table (no duplicates) */
static void hash_insert(seq_t v)
{
    if (hash_count * 2 >= hash_cap) hash_expand();

    /* simple hash: modulo a fixed prime */
    const seq_t PR = (seq_t)1234567;
    size_t h = (size_t)(v % PR) & (hash_cap - 1);

    while (hash_table[h].used) {
        if (hash_table[h].key == v) return;          /* already present */
        h = (h + 1) & (hash_cap - 1);
    }
    hash_table[h].used = true;
    hash_table[h].key  = v;
    hash_count++;
}

/* test whether a value is already in the hash table */
static bool hash_contains(seq_t v)
{
    const seq_t PR = (seq_t)1234567;
    size_t h = (size_t)(v % PR) & (hash_cap - 1);

    while (hash_table[h].used) {
        if (hash_table[h].key == v) return true;
        h = (h + 1) & (hash_cap - 1);
    }
    return false;
}

/* grow the hash table */
static void hash_expand(void)
{
    size_t new_cap = (hash_cap == 0 ? 16384 : hash_cap << 1);
    hentry_t *new_tab = (hentry_t *)malloc(new_cap * sizeof(hentry_t));
    if (!new_tab) exit(EXIT_FAILURE);
    for (size_t i = 0; i < new_cap; ++i) new_tab[i].used = false;

    /* rehash existing entries */
    for (size_t i = 0; i < hash_cap; ++i) {
        if (hash_table[i].used) {
            seq_t v = hash_table[i].key;
            const seq_t PR = (seq_t)1234567;
            size_t h = (size_t)(v % PR) & (new_cap - 1);
            while (new_tab[h].used) h = (h + 1) & (new_cap - 1);
            new_tab[h].used = true;
            new_tab[h].key  = v;
        }
    }
    free(hash_table);
    hash_table = new_tab;
    hash_cap   = new_cap;
}

/* -------------------------- sequence storage ---------------------------- */

static seq_t *seq      = NULL;  /* array of computed terms */
static size_t seq_cap   = 0;    /* capacity in terms of number of indices */
static size_t seq_len   = 0;    /* number of terms stored (seq_len > 0) */

/* ensure capacity for at least n+1 terms */
static void seq_ensure(size_t n)
{
    if (seq_cap >= n + 1) return;
    size_t new_cap = (seq_cap == 0 ? 16 : seq_cap << 1);
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_seq) exit(EXIT_FAILURE);
    if (seq) {
        for (size_t i = 0; i < seq_len; ++i) new_seq[i] = seq[i];
        free(seq);
    }
    seq = new_seq;
    seq_cap = new_cap;
}

/* ------------------------------ a(n) ------------------------------------ */

seq_t a(idx_t n)
{
    /* initialize on first call */
    if (seq_len == 0) {
        seq_ensure(0);
        seq[0] = (seq_t)0;
        seq_len = 1;

        hash_cap = 16384;
        hash_table = (hentry_t *)malloc(hash_cap * sizeof(hentry_t));
        if (!hash_table) exit(EXIT_FAILURE);
        for (size_t i = 0; i < hash_cap; ++i) hash_table[i].used = false;
        hash_insert((seq_t)0);  /* 0 is seen */
    }

    /* compute missing terms up to n */
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        seq_t prev = seq[i - 1];
        seq_t next = 0;
        bool found = false;

        /* --- compute binary length of prev --- */
        size_t len = 0;
        seq_t tmp = prev;
        do { len++; tmp >>= 1; } while (tmp != 0);

        /* --- enumerate all substrings of binary representation --- */
        for (size_t start = 0; start < len; ++start) {
            seq_t val = 0;
            for (size_t end = start; end < len; ++end) {
                seq_t bit = (prev >> (len - 1 - end)) & (seq_t)1;
                val = (val << 1) | bit;
                if (!hash_contains(val)) {
                    if (!found || val < next) {
                        next = val;
                        found = true;
                    }
                }
            }
        }

        if (!found) {
            next = prev * (seq_t)3 + (seq_t)1;
        }

        seq_ensure(i);
        seq[i] = next;
        seq_len = i + 1;
        hash_insert(next);
    }

    return seq[(size_t)n];
}
