/*
 * A188170  The number of divisors d of n of the form d == 3 (mod 8).
 * https://oeis.org/A188170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n with d ≡ 3 (mod 8)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    /* iterate d up to sqrt(n) without overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (d % 8 == 3) ++cnt;
            idx_t d2 = n / d;
            if (d2 != d && d2 % 8 == 3) ++cnt;
        }
    }
    return cnt;
}
