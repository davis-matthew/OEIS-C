/*
 * A215092  a(n) = Sum_{i=0..m} d(i)*3^i, where Sum_{i=0..m} d(i)*6^i is the
 *          base-6 representation of n.
 * https://oeis.org/A215092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{i>=0} d_i(n) * 3^i, where d_i are the base‑6 digits
 *            of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow = 1;
    for (idx_t x = n; x > 0; x /= 6) {
        int digit = x % 6;
        res += (seq_t)digit * pow;
        pow *= 3;
    }
    return res;
}
