/*
 * A064752  a(n) = n*6^n - 1.
 * https://oeis.org/A064752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*6^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 6^n using binary exponentiation */
    seq_t pow6 = 1;
    seq_t base  = (seq_t)6;
    idx_t  exp  = n;

    while (exp) {
        if (exp & 1U)  pow6 *= base;
        base *= base;
        exp >>= 1U;
    }

    /* Result = n * 6^n - 1 */
    return (seq_t)n * pow6 - (seq_t)1;
}
