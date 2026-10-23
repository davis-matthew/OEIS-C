/*
 * A144437  Period 3: repeat [3, 3, 1].
 * https://oeis.org/A144437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3/gcd(n,3) (period 3: [3, 3, 1])
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; return 1 when n is a multiple of 3, otherwise 3. */
    return (n % 3 == 0) ? (seq_t)1 : (seq_t)3;
}
