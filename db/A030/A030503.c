/*
 * A030503  Graham-Sloane-type lower bound on the size of a ternary (n,3,3)
 *          constant-weight code.
 * https://oeis.org/A030503
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2-7*n+8)/3 if n ≡ 1 (mod 3) * a(n) = (2*n^2-7*n+9)/3
 *            otherwise * (valid for all n ≥ 3)
 * Range:     u8 n <= 13, u16 n <= 314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return zero for indices below the offset; the caller should
       ensure n ≥ 3 for meaningful values. */
    if (n < 3)
        return (seq_t)0;

    /* cast n once to seq_t to avoid intermediate overflow in idx_t arithmetic */
    seq_t N = (seq_t)n;

    /* compute numerator: 2*n^2 - 7*n + (8 or 9) */
    seq_t num = (seq_t)2 * N * N;          /* 2*n^2 */
    num -= (seq_t)7 * N;                  /* subtract 7*n */
    if (n % 3 == 1) {
        num += (seq_t)8;
    } else {
        num += (seq_t)9;
    }

    /* integer division by 3 gives the desired value */
    return num / 3;
}
