/*
 * A102536  Number of triangles similar to their n-th pedal, and not similar
 *          to any k-th pedal for k < n.
 * https://oeis.org/A102536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * 2^d * (2^d - 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mobius, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of triangles similar to their n‑th pedal and not to any k‑th pedal for k < n. */
/* Uses helper functions from "mobius.h" and "pow2.h". */

#include "../utils/mobius.h"
#include "../utils/pow2.h"
#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* cumulative sums for positive and negative Mobius contributions */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor d1 = i, divisor d2 = n / i */
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* term for d1 */
            int mu1 = mobius(n / d1);
            if (mu1 != 0)
            {
                seq_t p1 = pow2(d1);           /* 2^d1 */
                seq_t psi1 = p1 * (p1 - 1);    /* psi(d1) */
                if (mu1 > 0)
                    sum_pos += psi1;
                else
                    sum_neg += psi1;           /* mu1 < 0 since mu1 != 0 */
            }

            /* term for d2 if distinct */
            if (d1 != d2)
            {
                int mu2 = mobius(n / d2);
                if (mu2 != 0)
                {
                    seq_t p2 = pow2(d2);
                    seq_t psi2 = p2 * (p2 - 1);
                    if (mu2 > 0)
                        sum_pos += psi2;
                    else
                        sum_neg += psi2;
                }
            }
        }
    }

    /* final result: positive sum minus negative sum (guaranteed non‑negative) */
    return sum_pos - sum_neg;
}
