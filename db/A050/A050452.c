/*
 * A050452  a(n) = Sum_{d|n, d == 3 (mod 4)} d.
 * https://oeis.org/A050452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d ≡ 3 (mod 4)} d
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) { /* out of range; but problem defines for n ≥ 1 */
        return 0;
    }
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= n / i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if ((d1 & 3) == 3) {
                sum += (seq_t)d1;
            }
            if (d2 != d1 && (d2 & 3) == 3) {
                sum += (seq_t)d2;
            }
        }
        ++i;
    }
    return sum;
}
