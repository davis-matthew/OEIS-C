/*
 * A198691  a(n) = 10*7^n-1.
 * https://oeis.org/A198691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*7^n-1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow7 = 1;    /* 7^n */
    seq_t base = 7;    /* base for exponentiation by squaring */
    idx_t exp = n;

    while (exp != 0u)
    {
        if (exp & 1u)
            pow7 *= base;
        base *= base;
        exp >>= 1u;
    }

    return 10 * pow7 - 1;
}
