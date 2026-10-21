/*
 * A211071  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant = 1 (mod 3).
 * https://oeis.org/A211071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^4 + 4*n^3 - 3*n^2 - 2*n - 7)/27 if n ≡ 1 (mod 3) *
 *            a(n) = (8*n^4 + 8*n^3 - 12*n^2 - 16*n - 4)/27 if n ≡ 2 (mod 3) *
 *            a(n) = 8*n^4 / 27 if n ≡ 0 (mod 3) * Offset: 1 (a(1) is the
 *            first term) * This implementation uses only integer arithmetic
 *            and requires no * additional storage or memois...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 345, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* not part of the defined offset */

    seq_t nn = (seq_t)n;            /* cast once to seq_t */
    seq_t nsq = nn * nn;            /* n² */
    seq_t ncu = nsq * nn;           /* n³ */
    seq_t n4  = ncu * nn;           /* n⁴ */

    seq_t numerator;
    idx_t mod = n % 3;

    if (mod == 0) {
        numerator = (seq_t)8 * n4;
    } else if (mod == 1) {
        numerator = (seq_t)8 * n4
                    + (seq_t)4 * ncu
                    - (seq_t)3 * nsq
                    - (seq_t)2 * nn
                    - (seq_t)7;
    } else { /* mod == 2 */
        numerator = (seq_t)8 * n4
                    + (seq_t)8 * ncu
                    - (seq_t)12 * nsq
                    - (seq_t)16 * nn
                    - (seq_t)4;
    }

    return numerator / (seq_t)27;   /* division is exact by design */
}
