/*
 * A003131  Decimal expansion of the order of Monster group.
 * https://oeis.org/A003131
 *
 * Offset:    54    (a(54) is the first term)
 * Keywords:  nonn, fini, full, nice, cons
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of the order of Monster group:
 * 2^46 · 3^20 · 5^9 · 7^6 · 11^2 · 13^3 · 17 · 19 · 23 · 29 · 31 · 41 · 47 · 59 · 67 · 71
 * = 808017424794512875886459904961710757005754368000000000 */

seq_t a(idx_t n) {
    static const char digits[] = "808017424794512875886459904961710757005754368000000000";
    if (n < 54 || n >= 108) return 0;
    return (seq_t)(digits[n - 54] - '0');
}
