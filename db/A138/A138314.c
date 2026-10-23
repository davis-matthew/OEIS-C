/*
 * A138314  E.g.f. satisfies: A(x) = exp( Sum_{n>=0} a(n)*x^(2n+1)/(2n+1)! ).
 * https://oeis.org/A138314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n-1)/2⌋} binomial(n-1,2k) * a(k) * a(n-2k-1)
 *            a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Public function returning the n‑th term of A138314 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_size = 0;   /* next index to compute (cache holds 0..cache_size-1) */

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            size_t new_cap = (cache_cap > 0 ? cache_cap << 1 : 16);
            if (new_cap < needed) new_cap = needed;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* compute terms from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {        /* a(0)=1 */
                cache[0] = 1;
                continue;
            }
            seq_t sum   = 0;
            const idx_t nm1 = i - 1;
            const idx_t maxk = nm1 / 2;
            seq_t binom = 1;                 /* binomial(n-1,0) */

            for (idx_t k = 0; k <= maxk; ++k) {
                /* term = binomial * a(k) * a(i-2k-1) */
                seq_t term = binom;
                term = term * cache[k];
                term = term * cache[i - 2 * k - 1];
                sum += term;

                /* update binomial for next k (if any) */
                if (k < maxk) {
                    /* C(n-1, 2(k+1)) = C(n-1, 2k) *
                       (n-1-2k)*(n-1-2k-1) /
                       ((2k+1)*(2k+2)) */
                    unsigned __int128 num = (unsigned __int128)(nm1 - 2 * k) *
                                            (unsigned __int128)(nm1 - 2 * k - 1);
                    unsigned __int128 den = (unsigned __int128)(2 * k + 1) *
                                            (unsigned __int128)(2 * k + 2);
                    binom = (seq_t)(( (unsigned __int128)binom * num ) / den);
                }
            }
            cache[i] = sum;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
