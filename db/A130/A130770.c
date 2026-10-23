/*
 * A130770  One third of the least common multiple of 3 and n^2+n+1.
 * https://oeis.org/A130770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 + n + 1) / gcd(3, n^2 + n + 1) * Since gcd(3, n^2 +
 *            n + 1) is 1 when n ≠ 1 (mod 3) and 3 otherwise, * the value is:
 *            n^2 + n + 1 if n % 3 != 1 * (n^2 + n + 1) / 3 if n % 3 == 1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * ((seq_t)n + 1) + 1;   /* compute n^2 + n + 1 */
    if (n % 3 == 1)
        return x / 3;
    else
        return x;
}
