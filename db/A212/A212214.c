/*
 * A212214  Number of representations of n as a sum of products of pairs of
 *          positive integers, n = Sum_{k=1..m} i_k*j_k with i_k<=j_k,
 *          i_k<=i_{k+1}, j_k<=j_{k+1}, i_k*j_k<=i_{k+1}*j_{k+1}.
 * https://oeis.org/A212214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 41, u32 n <= 126, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A212214
 *
 * Formula: a(n) = number of representations of n as a sum of
 * products of ordered pairs (i,j) with i <= j,
 * i_k <= i_{k+1}, j_k <= j_{k+1} and i_k*j_k <= i_{k+1}*j_{k+1}.
 *
 * The implementation uses memoised recursion on the triple
 * (n, i_start, j_start).  A simple open‑addressing hash table
 * holds previously computed results, allowing any order of calls
 * without recomputation.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external types provided by the build */
extern seq_t a(idx_t n);      /* prototype required by the build */

/*------------------------------------------------------------------*/
/* Simple hash table for memoisation                               */
/*------------------------------------------------------------------*/
typedef struct {
    uint64_t key;   /* 0 means empty (no state has key 0) */
    seq_t    val;
} ht_entry;

static ht_entry *ht      = NULL;
static size_t   ht_size  = 0;
static size_t   ht_used  = 0;
static const double HT_MAX_LOAD = 0.7;

/* initialise the table once */
static void ht_init(void)
{
    ht_size = 1 << 18;                 /* 262144 entries (power of two) */
    ht      = (ht_entry *)calloc(ht_size, sizeof(ht_entry));
}

/* 64‑bit multiplicative hash (Knuth) */
static inline uint64_t ht_hash(uint64_t k)
{
    return k * 11995408973635179863ULL;
}

/* grow the table when load factor threshold is exceeded */
static void ht_rehash(void)
{
    size_t new_size = ht_size << 1;
    ht_entry *new_ht = (ht_entry *)calloc(new_size, sizeof(ht_entry));

    for (size_t i = 0; i < ht_size; ++i) {
        if (ht[i].key != 0) {
            uint64_t k = ht[i].key;
            size_t h = ht_hash(k) & (new_size - 1);
            while (new_ht[h].key != 0) h = (h + 1) & (new_size - 1);
            new_ht[h].key = k;
            new_ht[h].val = ht[i].val;
        }
    }
    free(ht);
    ht = new_ht;
    ht_size = new_size;
    /* ht_used unchanged */
}

/* look up a stored value; returns 0 and *exists=0 if not found,
   otherwise returns stored value and *exists=1.  No insertion performed. */
static seq_t ht_lookup(uint64_t key, int *exists)
{
    if (ht == NULL) ht_init();

    size_t h = ht_hash(key) & (ht_size - 1);
    while (ht[h].key != 0) {
        if (ht[h].key == key) {
            if (exists) *exists = 1;
            return ht[h].val;
        }
        h = (h + 1) & (ht_size - 1);
    }
    if (exists) *exists = 0;
    return (seq_t)0;
}

/* insert a key/value pair into the table (key must not already exist). */
static void ht_insert(uint64_t key, seq_t val)
{
    if (ht == NULL) ht_init();

    /* ensure room before inserting */
    if (((double)(ht_used + 1) / ht_size) > HT_MAX_LOAD)
        ht_rehash();

    size_t h = ht_hash(key) & (ht_size - 1);
    while (ht[h].key != 0) h = (h + 1) & (ht_size - 1);

    ht[h].key = key;
    ht[h].val = val;
    ht_used++;

    if (((double)ht_used / ht_size) > HT_MAX_LOAD)
        ht_rehash();
}

/*------------------------------------------------------------------*/
/* Recursive counting routine                                       */
/*------------------------------------------------------------------*/
static seq_t count_rep(idx_t n, idx_t i_start, idx_t j_start)
{
    if (n == 0) return (seq_t)1;   /* empty sum */

    /* encode key: n in high 32 bits, i_start in next 16, j_start in low 16 */
    uint64_t key = (((uint64_t)n) << 32) | (((uint64_t)i_start) << 16) | (uint64_t)j_start;

    int exists = 0;
    seq_t cached = ht_lookup(key, &exists);
    if (exists) return cached;

    seq_t total = (seq_t)0;
    for (idx_t i = i_start; i * i <= n; ++i) {
        idx_t j_min = (i > j_start ? i : j_start);
        idx_t maxj  = n / i;
        for (idx_t j = j_min; j <= maxj; ++j) {
            idx_t w = i * j;
            total += count_rep(n - w, i, j);
        }
    }

    ht_insert(key, total);
    return total;
}

/*------------------------------------------------------------------*/
/* Public entry point                                               */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    return count_rep(n, 1, 1);
}
