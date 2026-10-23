/*
 * A132228  a(n) = Sum_{k=0..n} C(n-1,k)^2*a(k)*a(n-k-1) for n>0 with a(0)=1.
 * https://oeis.org/A132228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Σ_{k=0..n-1} C(n-1,k)^2 * a(k) * a(n-k-1) (n>0)
 *            (OEIS A132228)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, NULL */
#include <stddef.h>   /* size_t */

/* Forward declaration of helper that works in both C and C++ */
static void ensure_cache(idx_t n);

static seq_t *cache = NULL;   /* array of computed values, 0 = not yet computed */
static size_t cap   = 0;      /* current allocated capacity of cache */

/* C implementation of ensure_cache: uses realloc and simple zero-initialization */
static void ensure_cache_c(idx_t n)
{
    if (cache == NULL || cap <= n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Failure to alloc is ignored – the caller (a) will abort on it */
        cache = tmp;
        /* Zero‑initialize the newly allocated part */
        for (size_t i = cap; i < new_cap; ++i) {
            cache[i] = (seq_t)0;
        }
        cap = new_cap;
    }
}

/* C++ implementation of ensure_cache: uses new[] and copy to preserve
   object semantics (construction/destruction) */
#ifdef __cplusplus
static void ensure_cache_cpp(idx_t n)
{
    if (cache == NULL || cap <= n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = new seq_t[new_cap];
        /* move existing values, default‑construct the rest */
        for (size_t i = 0; i < cap; ++i) {
            tmp[i] = cache[i];
        }
        for (size_t i = cap; i < new_cap; ++i) {
            /* default construction gives zero for arithmetic types and
               default‑constructed arbitrary‑int objects give 0. */
        }
        delete[] cache;
        cache = tmp;
        cap = new_cap;
    }
}
#endif

/* Dispatcher that works for both languages */
static void ensure_cache(idx_t n)
{
#if defined(__cplusplus)
    ensure_cache_cpp(n);
#else
    ensure_cache_c(n);
#endif
}

/* Compute the n‑th term of the sequence */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Base case: a(0) = 1 */
    if (n == 0) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* Ensure that cache[0] is initialised */
    if (cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
    }

    /* Compute missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) {
            /* already computed */
            continue;
        }

        idx_t nm1 = i - 1;
        seq_t binom = (seq_t)1;   /* C(n-1,0) */
        seq_t sum = (seq_t)0;

        for (idx_t k = 0; k < i; ++k) {
            seq_t aik  = cache[k];
            seq_t aodd = cache[i - k - 1];

            seq_t term = binom * binom;   /* C(n-1,k)^2 */
            term = term * aik;
            term = term * aodd;

            sum = sum + term;

            /* Update binomial coefficient for next k */
            /* C(n-1, k+1) = C(n-1, k) * (n-1-k) / (k+1) */
            binom = (binom * (seq_t)(nm1 - k)) / (seq_t)(k + 1);
        }

        cache[i] = sum;
    }

    return cache[n];
}
