/*
 * A001492  Clock chimes with a quarter-hour bell.
 * https://oeis.org/A001492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 48) / 4 + 1 if (n mod 48) ≡ 3 (mod 4), else a(n) =
 *            1. Periodic with period 48
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n % 48;
    if (m % 4 == 3) {
        return (seq_t)(m / 4 + 1);
    } else {
        return 1;
    }
}
