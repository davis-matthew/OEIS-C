/*
 * A002984  a(0) = 1; for n > 0, a(n) = a(n-1) + floor(sqrt(a(n-1))).
 * https://oeis.org/A002984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + floor(sqrt(a(n-1))), a(0) = 1
 * Range:     u8 n <= 33, u16 n <= 517, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    seq_t s = 1; /* s = floor(sqrt(val)) */
    for (idx_t i = 1; i <= n; i++) {
        val += s;
        while (s + 1 <= val / (s + 1))
            s++;
    }
    return val;
}
