/*
 * A054861  Greatest k such that 3^k divides n!.
 * https://oeis.org/A054861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3) + floor(n/9) + floor(n/27) + … (exponent of 3
 *            in n!)
 * Range:     u8 n <= 515, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t t = n;
    while (t >= 3) {
        t /= 3;
        res += (seq_t)t;
    }
    return res;
}
