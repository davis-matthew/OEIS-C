/*
 * A010208  Continued fraction for sqrt(159).
 * https://oeis.org/A010208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=12; for n ≥ 1: a(n)=24 if n mod 10 = 0, a(n)=3 if n mod 10
 *            = 5, otherwise a(n)=1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u) return 12;
    idx_t r = n % 10u;
    if (r == 0u) return 24;
    if (r == 5u) return 3;
    return 1;
}
