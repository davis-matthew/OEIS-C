/*
 * A147818  Period 4: repeat [5, 9, 9, 5].
 * https://oeis.org/A147818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 5 if n mod 4 = 0 or 1, else 9
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n % 4;
    if (r == 0 || r == 1)
        return (seq_t)5;
    else
        return (seq_t)9;
}
