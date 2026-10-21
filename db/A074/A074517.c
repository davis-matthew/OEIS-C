/*
 * A074517  a(n) = 1^n + 5^n + 7^n.
 * https://oeis.org/A074517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 5^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5^n and 7^n by repeated multiplication.
       1^n is always 1, so the result is simply 1 + 5^n + 7^n. */
    seq_t pow5 = (seq_t)1;
    seq_t pow7 = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        pow5 *= (seq_t)5;
        pow7 *= (seq_t)7;
    }

    return pow5 + pow7 + (seq_t)1;
}
