/*
 * A230631  a(n) = n + (sum of digits in base-4 representation of n).
 * https://oeis.org/A230631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + (sum of digits in base-4 representation of n)
 * Range:     u8 n <= 246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;
    /* Add the base‑4 digits of n */
    while (x)
    {
        sum += (seq_t)(x & 3u); /* x mod 4 */
        x >>= 2;               /* floor(x / 4) */
    }
    return sum + (seq_t)n;
}
