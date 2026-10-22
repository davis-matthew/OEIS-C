/*
 * A007883  Westminster chimes at 15-minute intervals (2).
 * https://oeis.org/A007883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(n mod 4) if n mod 4 != 0; a(4k) = 16 + ((k-1) mod 12)
 *            + 1 (hour strikes cycle 1..12)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n % 4;
    if (r != 0) return (seq_t)(4 * r);
    idx_t k = n / 4;
    idx_t hour = (k - 1) % 12 + 1;
    return (seq_t)(16 + hour);
}
