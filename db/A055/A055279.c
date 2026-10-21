/*
 * A055279  Number of rooted trees with n nodes and 4 leaves.
 * https://oeis.org/A055279
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * x^5*(1 + x + 3x^2 + 5x^3 + 7x^4 +
 *            5x^5 + 5x^6 + 2x^7 + x^8) /* /((1 - x)^3 * (1 - x^2)^2 * (1 -
 *            x^3) * (1 - x^4))
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 132, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"

static const int Q[15] = { 1,-3,1,4,-3,1,-3,0,3,-1,3,-4,-1,3,-1 }; /* denominator coeffs */
static const seq_t P[14] = { 0,0,0,0,0, (seq_t)1, (seq_t)1, (seq_t)3,
                            (seq_t)5,(seq_t)7,(seq_t)5,(seq_t)5,
                            (seq_t)2,(seq_t)1 };          /* shifted numerator coeffs */

#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE+1];
static size_t computed = 5;                       /* indices 0..4 are already zero */

/* -------------------------------------------------------------------------- */
/*  Returns the nth term of A055279 (n≥5).  For n<5 the function returns 0.  */
seq_t a(idx_t n)
{
    if (n < 5)
        return 0;                                 /* undefined below offset */

    if ((size_t)n >= computed)
    {
        size_t limit = (size_t)n;
        for (size_t i = computed; i <= limit; ++i)
        {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int k = 1; k <= 14; ++k)
            {
                if (i < (size_t)k)
                    break;
                unsigned m = (Q[k] > 0) ? (unsigned)Q[k] : (unsigned)(-Q[k]);
                seq_t prod = mul_small(cache[i - k], m);
                if (Q[k] > 0)
                    sum_pos += prod;
                else if (Q[k] < 0)
                    sum_neg += prod;
            }
            seq_t tmp = (i < 14) ? P[i] : 0;
            seq_t val = tmp + sum_neg;
            val -= sum_pos;
            cache[i] = val;
        }
        computed = limit + 1;
    }
    return cache[n];
}
