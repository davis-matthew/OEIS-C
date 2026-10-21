/*
 * A092795  Number of connected relations.
 * https://oeis.org/A092795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n - 4*9^n - 3*7^n + 12*6^n - 6*5^n (offset = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 (outside the defined offset) return 0 for safety. */
    if (n == 0) return (seq_t)0;

    seq_t pow16 = (seq_t)1;
    seq_t pow9  = (seq_t)1;
    seq_t pow7  = (seq_t)1;
    seq_t pow6  = (seq_t)1;
    seq_t pow5  = (seq_t)1;

    /* Compute the required powers by successive multiplication. */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        pow16 *= (seq_t)16;
        pow9  *= (seq_t)9;
        pow7  *= (seq_t)7;
        pow6  *= (seq_t)6;
        pow5  *= (seq_t)5;
    }

    seq_t result = pow16;
    result -= (seq_t)4 * pow9;
    result -= (seq_t)3 * pow7;
    result += (seq_t)12 * pow6;
    result -= (seq_t)6 * pow5;

    return result;
}
