/*
 * A204354  Number of (n+2)X3 0..1 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically, diagonally or
 *          antidiagonally exactly one way, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A204354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204354: linear‑recurrence implementation
 *
 * Empirical recurrence (valid for n > 13):
 *   a(n) =  a(n‑1) + 3*a(n‑2) + 6*a(n‑3) + 21*a(n‑4)
 *          - 4*a(n‑5) -10*a(n‑6) + 2*a(n‑7) -20*a(n‑8) + 4*a(n‑10)
 *
 * Initial values (1‑based indices)
 *   a(1)=74,  a(2)=241,  a(3)=764,  a(4)=2399,  a(5)=7580,
 *   a(6)=23912, a(7)=75324, a(8)=237298, a(9)=747692,
 *   a(10)=2355866, a(11)=7422792, a(12)=23387588, a(13)=73689564
 *
 * The function memoises the results to support calls in any order.
 */

#include <stdlib.h>

/* Helper: allocate array of seq_t with given capacity (>=1).  */
static seq_t *alloc_seq_array(idx_t cap)
{
    return (seq_t *)malloc(sizeof(seq_t) * cap);
}

/* Memoization cache.  cache[0] is unused.  */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;  /* number of slots allocated (including index 0) */

/* Initial terms array.  Element 0 is unused.  */
static const seq_t init_terms[14] = {
    (seq_t)0,
    (seq_t)74,      /* a(1) */
    (seq_t)241,     /* a(2) */
    (seq_t)764,     /* a(3) */
    (seq_t)2399,    /* a(4) */
    (seq_t)7580,    /* a(5) */
    (seq_t)23912,   /* a(6) */
    (seq_t)75324,   /* a(7) */
    (seq_t)237298,  /* a(8) */
    (seq_t)747692,  /* a(9) */
    (seq_t)2355866, /* a(10) */
    (seq_t)7422792, /* a(11) */
    (seq_t)23387588,/* a(12) */
    (seq_t)73689564 /* a(13) */
};

/* Ensure the cache contains values up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;

    /* We need storage for indices 0..n inclusive.  */
    idx_t need = n + 1;
    if (cache_size >= need) return;

    /* Allocate new larger array.  */
    seq_t *new_cache = alloc_seq_array(need);
    if (!new_cache) return;          /* memory error – leave cache unchanged */

    /* Copy existing values.  */
    for (idx_t i = 1; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* Initialize known initial terms.  */
    for (idx_t i = 1; i <= 13 && i < need; ++i)
        if (new_cache[i] == (seq_t)0)
            new_cache[i] = init_terms[i];

    /* Compute remaining terms using the recurrence.  */
    for (idx_t i = cache_size; i < need; ++i)
    {
        if (i <= 13)           /* already initialised, skip */
            continue;

        seq_t val = new_cache[i-1];
        val += (seq_t)3 * new_cache[i-2];
        val += (seq_t)6 * new_cache[i-3];
        val += (seq_t)21 * new_cache[i-4];
        val -= (seq_t)4 * new_cache[i-5];
        val -= (seq_t)10 * new_cache[i-6];
        val += (seq_t)2 * new_cache[i-7];
        val -= (seq_t)20 * new_cache[i-8];
        val += (seq_t)4 * new_cache[i-10];
        new_cache[i] = val;
    }

    /* Replace old cache.  */
    free(cache);
    cache      = new_cache;
    cache_size = need;
}

/* Public function.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
