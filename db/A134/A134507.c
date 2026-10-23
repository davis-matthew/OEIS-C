/*
 * A134507  Number of rectangles in a pyramid built with squares. The squares
 *          counted in A092498 are excluded.
 * https://oeis.org/A134507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   A134507 a(n) = { n*(3*n^3+5*n^2-3*n-3)/18 if n ≡ 0 (mod 3)
 *            (n-1)*(3*n^3+8*n^2+5*n+2)/18 if n ≡ 1 (mod 3)
 *            (3*n^4+5*n^3-3*n^2-3*n+2)/18 if n ≡ 2 (mod 3)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 197, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is zero‑based in C, but the OEIS offset is 1; the formulas
       below are valid for all n ≥ 1.  For n = 0 the result is defined
       as 0 (though it will never be requested by the tests).        */
    if (n == 0) return 0;

    seq_t N = (seq_t)n;          /* convert to the arithmetic type used
                                    for the sequence values          */
    seq_t res;

    switch (n % 3) {
    case 0: {
        /* n = 3k */
        seq_t t = 3*N*N*N + 5*N*N - 3*N - 3;
        res = N * t / 18;
        break;
    }
    case 1: {
        /* n = 3k+1 */
        seq_t t = 3*N*N*N + 8*N*N + 5*N + 2;
        res = (N - 1) * t / 18;
        break;
    }
    default: /* n = 3k+2 */
        {
        seq_t t = 3*N*N*N*N + 5*N*N*N - 3*N*N - 3*N + 2;
        res = t / 18;
        break;
        }
    }

    return res;
}
