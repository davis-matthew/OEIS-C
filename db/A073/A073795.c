/*
 * A073795  Replace 8^k with (-8)^k in base 8 expansion of n.
 * https://oeis.org/A073795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = sum_{k=0}^{∞} (digit_k of n in base 8) × (-8)^k
 *            Equivalently: a(8·k+m) = -8·a(k) + m for 0 ≤ m < 8
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    idx_t x = n;
    while (x)
    {
        seq_t digit = (seq_t)(x & 7);   /* x % 8 */
        result += digit * power;
        power *= (seq_t)-8;             /* multiply by -8 */
        x >>= 3;                        /* x /= 8 */
    }
    return result;
}
