/*
 * A180972  Numbers n such that 8^9 + n^2 is a square.
 * https://oeis.org/A180972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = 2^(13 - n) * (2^(2n - 1) - 1) for n = 1..13; a(n) = 0 for
 *            n > 13 (sequence has only 13 terms)
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is defined only for 1 ≤ n ≤ 13 */
    if (n == 0 || n > 13)
        return 0;

    /* Compute the powers of two with safe shift counts */
    unsigned int shift1 = 13U - (unsigned int)n;
    unsigned int shift2 = 2U * (unsigned int)n - 1U;

    /* Compute 2^shift1 */
    seq_t t1 = ((seq_t)1) << shift1;
    /* Compute 2^shift2 */
    seq_t t2 = ((seq_t)1) << shift2;

    /* a(n) = t1 * (t2 - 1) */
    return t1 * (t2 - 1);
}
