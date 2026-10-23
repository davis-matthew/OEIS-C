/*
 * A009943  Coordination sequence for NiAs(1), As position.
 * https://oeis.org/A009943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=6*n^2 + 2 if n is odd and n>1, otherwise
 *            a(n)=6*n^2. This yields a(1)=6
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 in seq_t domain          */
    seq_t res = (seq_t)6 * nn;               /* 6*n^2                         */

    if ((n & 1U) && (n > 1)) {              /* odd n and n>1: add 2         */
        res += (seq_t)2;
    }

    return res;
}
