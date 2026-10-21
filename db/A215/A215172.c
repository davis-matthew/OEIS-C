/*
 * A215172  a(0)=1, a(n) = a(n-1)*4^n + 2^n - 1. That is, add n 0's and n 1's
 *          to the binary representation of previous term.
 * https://oeis.org/A215172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1; a(n)=a(n-1)*4^n + 2^n - 1, for n≥1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* a(0) */
    if (n == 0)
        return result;

    /* 4^i and 2^i are built incrementally: start with i = 1. */
    seq_t pow4 = 4;            /* 4^1 */
    seq_t pow2 = 2;            /* 2^1 */

    for (idx_t i = 1; i <= n; ++i) {
        result = result * pow4 + pow2 - 1;  /* a(i)   */
        pow4 *= 4;                         /* 4^(i+1) */
        pow2 *= 2;                         /* 2^(i+1) */
    }

    return result;
}
