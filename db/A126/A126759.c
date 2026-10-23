/*
 * A126759  a(0) = 1; a(2n) = a(n); a(3n) = a(n); otherwise write n = 6i+j,
 *          where j = 1 or 5 and set a(n) = 2i+2 if j = 1, otherwise a(n) =
 *          2i+3.
 * https://oeis.org/A126759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, hear
 * Formula:   a(0) = 1; a(2n) = a(n); a(3n) = a(n); a(6i+1) = 2i + 2; a(6i+5)
 *            = 2i + 3
 * Range:     u8 n <= 762, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t x = n;
    /* strip all powers of 2 */
    while ((x & 1ULL) == 0)
        x >>= 1;

    /* strip all powers of 3 */
    while (x % 3 == 0)
        x /= 3;

    /* now x is odd and not divisible by 3, i.e. x % 6 == 1 or 5 */
    if (x % 6 == 1) {
        return (seq_t)((seq_t)(x / 6) * 2 + 2);
    } else { /* x % 6 == 5 */
        return (seq_t)((seq_t)(x / 6) * 2 + 3);
    }
}
