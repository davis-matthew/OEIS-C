/*
 * A170617  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)= 30*(a(n-1)+…+a(max(0,n-47))) +
 *            (n≥48?465·a(n-48):0) + c(n) where c(n)=1 if n=0, c(n)=2 for
 *            1≤n≤47, c(n)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies the rational generating function
   G(t)= (t^48+2t^47+…+2t+1)/(465t^48-30t^47-…-30t-1).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* a[0..computed-1] */
    static size_t computed = 0;   /* number of elements currently valid */
    static size_t capacity = 0;   /* allocated size of cache */

    /* Grow the cache if necessary.  */
    if ((size_t)n + 1 > computed) {
        size_t new_cap = capacity ? capacity * 2 : 1;
        while (new_cap < (size_t)n + 1) {
            new_cap *= 2;
        }

        /* realloc returns void*, cast it to seq_t* for C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – the caller will see 0 */
            return (seq_t)0;
        }
        cache = tmp;
        capacity = new_cap;

        /* Compute new terms from computed up to n */
        for (idx_t i = computed; i <= n; ++i) {
            seq_t sum = 0;

            /* Add 30 times the previous up to 47 terms  */
            idx_t limit = i < 47 ? i : 47;
            for (idx_t k = 1; k <= limit; ++k) {
                sum += cache[(size_t)(i - k)] * (seq_t)30;
            }

            /* Add 465 times the 48‑th previous term if available */
            if (i >= 48) {
                sum += cache[(size_t)(i - 48)] * (seq_t)465;
            }

            /* Constant term */
            if (i == 0) {
                sum += (seq_t)1;
            } else if (i <= 47) {
                sum += (seq_t)2;
            }

            cache[(size_t)i] = sum;
        }
        computed = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
