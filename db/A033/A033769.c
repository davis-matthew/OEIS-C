/*
 * A033769  Product t2(q^d); d | 10, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 611, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A033769:  a(n) = coefficient of q^n in ∏_{d|10} t2(q^d),
    where t2(q)=∑_{k≥0} q^{k(k+1)/2}.  This is equivalent to
    counting non‑negative integer solutions of
        Σ_{d∈{1,2,5,10}} d * k_d(k_d+1)/2 = n.  */
#include <stdlib.h>
#include <stddef.h>

/* Cached sequence values.  Recomputes when a larger n is requested. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Return the n-th term.  n may come in any order. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n+1 terms cached. */
    if (n + 1 > cache_len) {
        /* Allocate array for new limit. */
        size_t N = (size_t)(n + 1);           /* array length = n+1 */
        seq_t *arr = (seq_t *)calloc(N, sizeof(seq_t));
        if (!arr) return 0;                    /* out of memory -> 0 */

        arr[0] = (seq_t)1;                    /* initial product = 1 */

        /* Divisors of 10. */
        const idx_t divs[] = {1, 2, 5, 10};
        const size_t nd = sizeof(divs) / sizeof(divs[0]);

        for (size_t di = 0; di < nd; ++di) {
            idx_t d = divs[di];

            /* Compute triangular numbers scaled by d up to N-1. */
            size_t tcap = 16;
            idx_t *tvals = (idx_t *)malloc(tcap * sizeof(idx_t));
            if (!tvals) { free(arr); return 0; }
            size_t tcount = 0;
            idx_t k = 0, t;
            while (1) {
                t = d * k * (k + 1) / 2;
                if (t >= N) break;
                if (tcount >= tcap) {
                    tcap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(tvals, tcap * sizeof(idx_t));
                    if (!tmp) { free(tvals); free(arr); return 0; }
                    tvals = tmp;
                }
                tvals[tcount++] = t;
                ++k;
            }

            /* Convolution with the current divisor's series. */
            seq_t *newarr = (seq_t *)calloc(N, sizeof(seq_t));
            if (!newarr) { free(tvals); free(arr); return 0; }
            for (size_t ti = 0; ti < tcount; ++ti) {
                idx_t shift = tvals[ti];
                for (idx_t idx = shift; idx < N; ++idx) {
                    newarr[idx] += arr[idx - shift];
                }
            }

            free(tvals);
            free(arr);                     /* free previous array */
            arr = newarr;                  /* use new array for next divisor */
        }

        /* Store the computed values in the cache. */
        free(cache);
        cache = arr;
        cache_len = N;
    }

    return cache[n];
}
