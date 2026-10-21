/*
 * A078533  Coefficients of power series that satisfies A(x)^4 - 16x*A(x)^5 =
 *          1, A(0)=1.
 * https://oeis.org/A078533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A078533 - Coefficients of the power series that satisfies
 *    A(x)^4 - 16x*A(x)^5 = 1,  A(0) = 1
 *
 * Closed form:
 *    a(n) = 4^n * ∏_{k=0}^{n-1} (5n - 3 - 4k)  / (n! * (n+1))
 *
 * The implementation uses only integer arithmetic.  Intermediate
 * products are reduced by gcds so that 64‑bit builds never overflow
 * if the final answer fits in 64 bits.  The algorithm is O(n)
 * and works for arbitrarily large n when seq_t has arbitrary width.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t n_seq = (seq_t)n;

    /* ---- Step 1 : Compute the numerator product
                ∏ (5n - 3 - 4k)  and cancel with n! immediately  ---- */
    seq_t numer = 1;    /* product of the terms above  */
    seq_t denom = 1;    /* product of 1..n                 */

    for (idx_t k = 0; k < n; ++k) {
        seq_t term = 5 * n_seq - 3 - 4 * (seq_t)k;
        numer *= term;
        denom *= (k + 1);         /* multiply by k+1 for n! */

        /* cancel common factors to keep numbers small */
        seq_t g = gcd(numer, denom);
        numer /= g;
        denom /= g;
    }

    /* multiply by the extra factor (n+1) in the denominator   */
    denom *= (n_seq + 1);
    seq_t g = gcd(numer, denom);
    numer /= g;
    denom /= g;

    /* ---- Step 2 : Multiply by 4^n, cancelling with remaining denominator ---- */
    seq_t result = numer;      /* current result                */
    seq_t factor  = 4;         /* 4^1 for the first multiplication */

    for (idx_t i = 0; i < n; ++i) {
        /* cancel current factor with denom first */
        g = gcd(factor, denom);
        factor /= g;
        denom /= g;

        /* multiply by the (reduced) factor */
        result *= factor;

        /* cancel any new common factors */
        g = gcd(result, denom);
        result /= g;
        denom /= g;

        /* prepare factor for the next iteration */
        factor = 4;
    }

    /* At this point denom should be 1.  If not, cancel it.            */
    if (denom != 1) {
        seq_t g2 = gcd(result, denom);
        result /= g2;
        denom   /= g2;
        /* denom must now be 1; otherwise the sequence is not integral.  */
    }

    return result;
}
