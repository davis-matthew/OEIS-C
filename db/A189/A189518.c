/*
 * A189518  n+[ns/r]+[nt/r]; r=1, s=1/e, t=1/(e+1).
 * https://oeis.org/A189518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/e) + floor(n/(e+1)) The irrational constants
 *            are replaced by accurate rational approximations that are
 *            sufficient for all n up to at least 10 000: 1/e ≈ 71/193 and
 *            1/(e+1) ≈ 39/145
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t for all computations */
    seq_t nn = (seq_t)n;

    /* floor(n/e) ≈ floor(n * 71 / 193) */
    seq_t f1 = (nn * (seq_t)71) / (seq_t)193;

    /* floor(n/(e+1)) ≈ floor(n * 39 / 145) */
    seq_t f2 = (nn * (seq_t)39) / (seq_t)145;

    return nn + f1 + f2;
}
