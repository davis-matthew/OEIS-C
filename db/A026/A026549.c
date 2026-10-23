/*
 * A026549  Ratios of successive terms are 2, 3, 2, 3, 2, 3, 2, 3, ...
 * https://oeis.org/A026549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(3 - (-1)^n)*6^floor(n/2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(2k) = 6^k, a(2k+1) = 2*6^k */

seq_t a(idx_t n)
{
    idx_t exp = n >> 1;           /* floor(n/2) */
    seq_t pow6 = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow6 *= (seq_t)6;          /* compute 6^exp */

    if (n & 1)                     /* if n is odd, multiply by 2 */
        pow6 *= (seq_t)2;

    return pow6;
}
