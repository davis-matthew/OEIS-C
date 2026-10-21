/*
 * A078486  Expansion of (x-7*x^2+19*x^3-21*x^4+10*x^5-6*x^6) /
 *          (1-9*x+31*x^2-53*x^3+44*x^4-16*x^5+6*x^6).
 * https://oeis.org/A078486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in the power series of (x - 7x^2 +
 *            19x^3 - 21x^4 + 10x^5 - 6x^6) / (1 - 9x + 31x^2 - 53x^3 + 44x^4
 *            - 16x^5 + 6x^6)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cache_len = 0;  /* number of computed terms */

    /* Extend cache up to n, if necessary */
    if ((size_t)n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        /* Allocate or enlarge the cache */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Compute missing terms */
        for (size_t i = cache_len; i < new_len; ++i)
        {
            seq_t val = 0;

            if (i == 0)
            {
                val = 0;               /* a(0) = 0 */
            }
            else
            {
                seq_t pos = 0;          /* sum of positive parts */
                seq_t neg = 0;          /* sum of negative parts */

                /* Contribution of the numerator b(i) (non‑zero only for i≤6) */
                if (i <= 6)
                {
                    int bval = 0;
                    switch (i)
                    {
                        case 1: bval = 1;   break;
                        case 2: bval = -7;  break;
                        case 3: bval = 19;  break;
                        case 4: bval = -21; break;
                        case 5: bval = 10;  break;
                        case 6: bval = -6;  break;
                    }
                    if (bval > 0) pos += (seq_t)bval;
                    else          neg += (seq_t)(-bval);
                }

                /* Coefficients of the recurrence:
                   a(n) = 9 a(n‑1) – 31 a(n‑2) + 53 a(n‑3) – 44 a(n‑4) + 16 a(n‑5) – 6 a(n‑6)
                   Index i in the array corresponds to n in the recurrence. */
                const int coeff[7] = {0, 9, -31, 53, -44, 16, -6};

                for (int off = 1; off <= 6; ++off)
                {
                    if (i < (size_t)off) continue;   /* term not defined yet */
                    seq_t term = cache[i - off];
                    int c = coeff[off];
                    if (c > 0) pos += term * (seq_t)c;
                    else       neg += term * (seq_t)(-c);
                }

                /* Final value: pos – neg  (pos ≥ neg for all n) */
                val = pos - neg;
            }

            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
