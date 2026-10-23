/*
 * A038734  T(n,n-5), array T as in A038730.
 * https://oeis.org/A038734
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = coefficient of x^n in
 *              x^5 / ((1 - 3x + x^2)(1 - x)^9).
 *
 *  The denominator yields a linear recurrence of order 11:
 *      a(n) = 12*a(n-1) - 64*a(n-2) + 201*a(n-3) - 414*a(n-4)
 *             + 588*a(n-5) - 588*a(n-6) + 414*a(n-7) - 201*a(n-8)
 *             +  64*a(n-9) - 12*a(n-10) + a(n-11)
 *  (valid for all n ≥ 5, with a(k)=0 for k<5 and a(5)=1).
 *
 *  This implementation memoises already‑computed terms so that
 *  calls may arrive in any order.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache of computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* capacity of the cache array */
static idx_t cache_max  = 4;   /* highest index that is guaranteed to be computed (a(0)…a(4)=0) */

/* Ensure that the cache can hold values up to n */
static void ensure_cache_size(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = cache_size ? cache_size * 2 : 64;
        if (new_size <= n) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                        /* pragma: not 1 {
                                                   abort(); } */
        /* zero initialise any new part */
        memset(tmp + cache_size, 0,
               (new_size - cache_size) * sizeof(seq_t));
        cache     = tmp;
        cache_size = new_size;
    }
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    if (n < 5)            /* sequence is defined only for n ≥ 5 */
        return 0;

    /* Make sure the cache array is large enough */
    ensure_cache_size(n);

    /* Compute any values that have not yet been memoised */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i == 5) {                      /* seed value */
                cache[i] = 1;
                continue;
            }

            seq_t val = 0;
            /* Recurrence: only terms with index >= 5 are non‑zero */
            for (int k = 1; k <= 11; ++k) {
                idx_t idx = i - k;
                if (idx < 5) break;            /* values for k too large use 0 */
                switch (k) {
                case 1:  val += (seq_t)12 *  cache[idx]; break;
                case 2:  val -= (seq_t)64 *  cache[idx]; break;
                case 3:  val += (seq_t)201 * cache[idx]; break;
                case 4:  val -= (seq_t)414 * cache[idx]; break;
                case 5:  val += (seq_t)588 * cache[idx]; break;
                case 6:  val -= (seq_t)588 * cache[idx]; break;
                case 7:  val += (seq_t)414 * cache[idx]; break;
                case 8:  val -= (seq_t)201 * cache[idx]; break;
                case 9:  val += (seq_t)64  * cache[idx]; break;
                case 10: val -= (seq_t)12  * cache[idx]; break;
                case 11: val +=            cache[idx];    break;
                }
            }
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
