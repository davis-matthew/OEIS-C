/*
 * A007879  Chimes made by clock striking the hour and half-hour.
 * https://oeis.org/A007879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + ((n-1)*(n mod 2)/2) mod 12
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return 1 + (((n - 1) * (n % 2) / 2) % 12);
}
