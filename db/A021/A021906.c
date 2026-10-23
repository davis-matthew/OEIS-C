/*
 * A021906  Decimal expansion of 1/902.
 * https://oeis.org/A021906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = 0 for n = 0, 1; otherwise a(n) = period[(n-2) mod 10]
 *            where period = {1, 1, 0, 8, 6, 4, 7, 4, 5, 0}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;
    static const seq_t period[10] = {1, 1, 0, 8, 6, 4, 7, 4, 5, 0};
    idx_t idx = (n - 2) % 10;   /* index in the period */
    return period[idx];
}
