/*
 * A194130  a(n) = n!/gcd(n,3).
 * https://oeis.org/A194130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / gcd(n,3)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    int need_div3 = (n % 3 == 0);

    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = i;
        if (need_div3 && (i % 3 == 0)) {
            factor /= 3;      /* cancel the single factor of 3 */
            need_div3 = 0;
        }
        result *= factor;
    }
    return result;
}
