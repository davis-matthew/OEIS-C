/*
 * A054460  A001333(n), n >= 1, convolved twice with itself.
 * https://oeis.org/A054460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A054460 – Convolution of A001333 with itself
   Formula (linear recurrence):
   a(0)=1, a(1)=9, a(2)=48, a(3)=204, a(4)=765, a(5)=2649,
   a(n)=6*a(n-1)-9*a(n-2)-4*a(n-3)+9*a(n-4)+6*a(n-5)+a(n-6)   (n≥6)
*/

#include <stdlib.h>

/* Utility allocator that works for both C (malloc) and C++ (new) */
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Memoised cache of computed terms */
static seq_t *cache = NULL;      /* array of a(i) */
static idx_t  cache_len = 0;     /* number of terms stored (cache_len = last_index+1) */

/* Pre‑defined initial values for n = 0 … 5 */
static const seq_t init_vals[6] = {
    (seq_t)1,   /* a(0) */
    (seq_t)9,   /* a(1) */
    (seq_t)48,  /* a(2) */
    (seq_t)204, /* a(3) */
    (seq_t)765, /* a(4) */
    (seq_t)2649/* a(5) */
};

/* Ensure that cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)              /* already computed */
        return;

    idx_t   new_len = n + 1;        /* number of elements needed */
    seq_t * new_cache = alloc_seq_array(new_len);

    /* Copy old values */
    for (size_t i = 0; i < (size_t)cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute remaining values */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i < 6) {
            new_cache[i] = init_vals[(size_t)i];
        } else {
            /* a(n)=6*a(n-1)-9*a(n-2)-4*a(n-3)+9*a(n-4)+6*a(n-5)+a(n-6) */
            seq_t term =
                ((seq_t)6 * new_cache[i-1])
              - ((seq_t)9 * new_cache[i-2])
              - ((seq_t)4 * new_cache[i-3])
              + ((seq_t)9 * new_cache[i-4])
              + ((seq_t)6 * new_cache[i-5])
              + new_cache[i-6];
            new_cache[i] = term;
        }
    }

    /* Replace old cache */
    free_seq_array(cache);
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
