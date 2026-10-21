/*
 * A061295  Minimal number of steps to get from 0 to n by (a) adding 1 or (b)
 *          subtracting 1 or (c) multiplying by 3.
 * https://oeis.org/A061295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 2 a(3n) = a(n) + 1 a(3n+1) = a(n) + 2
 *            a(3n+2) = a(n+1) + 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    if (n % 3 == 0) {
        return a(n / 3) + 1;
    } else if (n % 3 == 1) {
        return a(n / 3) + 2;
    } else {                      /* n % 3 == 2 */
        return a(n / 3 + 1) + 2;
    }
}
