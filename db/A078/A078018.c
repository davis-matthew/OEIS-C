/*
 * A078018  a(n) = Sum_{k=0..n} 6^k*N(n,k), with a(0)=1, where N(n,k) = C(n,k)
 *          * C(n,k+1)/n are the Narayana numbers (A001263).
 * https://oeis.org/A078018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 6^k * N(n,k) * more efficiently computed by
 *            the recurrence * a(0)=a(1)=1 * a(n) = (7*(2*n-1)*a(n-1) -
 *            25*(n-2)*a(n-2)) / (n+1) for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of previously computed values */
static seq_t *seq_cache   = NULL;   /* array of values 0..seq_len-1 */
static idx_t  seq_len     = 0;      /* number of values stored */
static idx_t  seq_cap     = 0;      /* capacity of seq_cache */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise cache with first two terms */
    if (seq_cache == NULL) {
        seq_cap = 2;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;          /* allocation failed */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_len   = 2;
    }

    /* expand cache if necessary */
    if (n >= seq_len) {
        /* grow capacity until it can hold n+1 entries */
        idx_t new_cap = seq_cap;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* allocation failed */
        seq_cache = tmp;
        seq_cap   = new_cap;

        /* compute missing terms iteratively */
        for (idx_t i = seq_len; i <= n; ++i) {
            /* compute numerator = 7*(2*i-1)*a[i-1] - 25*(i-2)*a[i-2] */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
            /* portable 128‑bit type is available in C99/11 compilers */
            unsigned __int128 term1 = (unsigned __int128)seq_cache[i-1]
                                    * (unsigned __int128)7
                                    * (unsigned __int128)(2*i - 1);
            unsigned __int128 term2 = (unsigned __int128)seq_cache[i-2]
                                    * (unsigned __int128)25
                                    * (unsigned __int128)(i-2);
            unsigned __int128 numer = term1 - term2;
            seq_cache[i] = (seq_t)(numer / (i+1));
#else
            /* if 128‑bit not available, fall back to seq_t arithmetic
               (this works for big‑integer implementations) */
            seq_t term1 = seq_cache[i-1] * (seq_t)7 * (seq_t)(2*i - 1);
            seq_t term2 = seq_cache[i-2] * (seq_t)25 * (seq_t)(i-2);
            seq_cache[i] = (term1 - term2) / (i+1);
#endif
        }
        seq_len = n + 1;
    }

    return seq_cache[n];
}
