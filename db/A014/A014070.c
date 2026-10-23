/*
 * A014070  a(n) = binomial(2^n, n).
 * https://oeis.org/A014070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2^n, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/gcd.h"

/* Computes a(n) = C(2^n, n) for all n >= 0.
 * The routine works for arbitrary‑precision seq_t as well as for
 * uint8_t…uint64_t.  For the 64‑bit build it never overflows,
 * because the numerator and denominator are reduced by their GCD
 * before each multiplication. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t N   = pow2(n);   /* N = 2^n  */
    seq_t res = 1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t num = N - (k - 1); /* numerator factor   */
        seq_t den = k;           /* denominator factor */

        /* Reduce res with den */
        seq_t g = gcd(res, den);
        res  /= g;
        den  /= g;

        /* Reduce num with remaining den */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* After the two reductions den must be 1,
           otherwise the division would not be exact. */
        res = res * num;   /* multiplication is safe due to reductions */
        if (den != 1)
            res /= den;   /* for completeness – should never happen */
    }

    return res;
}
