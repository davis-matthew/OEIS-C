/*
 * A200433  Number of -n..n arrays x(0..5) of 6 elements with zero sum and no
 *          two or three adjacent elements summing to zero.
 * https://oeis.org/A200433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (264*n^5 - 545*n^4 + 660*n^3 - 535*n^2 + 156*n) / 15
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 30, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1   (a(1) = 0) */

seq_t a(idx_t n)
{
    /* n is zero or larger; behaviour for n=0 is not required here. */
    if (n == 0) return 0;

    /* Convert index to seq_t type */
    seq_t n1 = (seq_t)n;

    /* Compute powers of n using only multiplication by small integers */
    seq_t n2 = n1 * n;          /* n^2 */
    seq_t n3 = n2 * n;          /* n^3 */
    seq_t n4 = n3 * n;          /* n^4 */
    seq_t n5 = n4 * n;          /* n^5 */

    /* Positive part of numerator: 264*n^5 + 660*n^3 + 156*n */
    seq_t pos = n5 * 264 + n3 * 660 + n1 * 156;

    /* Negative part of numerator: 545*n^4 + 535*n^2 */
    seq_t neg = n4 * 545 + n2 * 535;

    /* Combine and divide by 15 (exact division) */
    seq_t num = pos - neg;
    return num / (seq_t)15;
}
