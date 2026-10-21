/*
 * A088439  a(3n) = 3n, otherwise a(n) = 1.
 * https://oeis.org/A088439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n mod 3 == 0, else 1
 * Range:     u8 n <= 257, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n % 3) == 0)
        return (seq_t)n;
    else
        return (seq_t)1;
}
