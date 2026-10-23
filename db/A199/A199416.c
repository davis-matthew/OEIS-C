/*
 * A199416  a(n) = 2*7^n+1.
 * https://oeis.org/A199416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*7^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power = 1;    /* 7^0 */
    seq_t base  = 7;    /* base 7 */

    /* fast exponentiation by squaring */
    while (n > 0)
    {
        if (n & 1)
            power = power * base;
        base = base * base;
        n >>= 1;
    }

    /* a(n) = 2*7^n + 1 */
    return (power * 2) + 1;
}
