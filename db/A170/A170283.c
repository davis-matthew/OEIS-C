/*
 * A170283  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff(n) + 32 * Σ_{k=1}^{40} a(n-k) - 528 * a(n-41) where
 *            coeff(0)=1, coeff(1..40)=2, coeff(41)=1, coeff(n>41)=0. This
 *            follows from the rational generating function of A170283
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute a(n) for the Coxeter group on 34 generators with (S_i)^2 = (S_i S_j)^41 = I */
seq_t a(idx_t n)
{
    /* static cache for memoisation: cache[i] == a(i)   */
    static seq_t *cache = NULL;
    static size_t  used  = 0;   /* number of elements currently stored (cache[0..used-1]) */
    static size_t  cap   = 0;   /* allocated capacity */

    /* target index as size_t (n is guaranteed <= 10000 in tests) */
    size_t t = (size_t)n;

    /* enlarge cache if necessary */
    if (t >= used) {
        /* determine required capacity: at least t+1, grow by powers of two for amortised O(1) */
        size_t needed = t + 1;
        if (cap < needed) {
            size_t newcap = (cap == 0) ? 1 : cap;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) { /* allocation failure, abort to avoid UB */
                abort();
            }
            cache = tmp;
            cap   = newcap;
        }

        /* compute missing terms from 'used' up to 't' */
        for (size_t i = used; i <= t; ++i) {
            /* coefficient derived from numerator of g.f. */
            seq_t coeff;
            if (i == 0)          coeff = (seq_t)1;
            else if (i <= 40)    coeff = (seq_t)2;
            else if (i == 41)    coeff = (seq_t)1;
            else                 coeff = (seq_t)0;

            /* recurrence part */
            seq_t sum = coeff;
            /* add 32 * a(i-k) for k = 1..40 as long as i >= k */
            for (int k = 1; k <= 40; ++k) {
                if (i >= (size_t)k)
                    sum += (seq_t)32 * cache[i - k];
            }
            /* subtract 528 * a(i-41) if exists */
            if (i >= 41)
                sum -= (seq_t)528 * cache[i - 41];

            cache[i] = sum;
        }
        used = t + 1;
    }

    return cache[t];
}
