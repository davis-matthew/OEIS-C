/*
 * A021150  Decimal expansion of 1/146.
 * https://oeis.org/A021150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-1) - a(n-4) + a(n-5); a(0) = 0, a(1) = 0, a(2) = 6,
 *            a(3) = 8, a(4) = 4, a(5) = 9 (The sequence is 8‑periodic after
 *            the first two zeros.)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;               /* a(0) = a(1) = 0 */
    static const seq_t block[8] = {6, 8, 4, 9, 3, 1, 5, 0};
    unsigned idx = (unsigned)((n - 2) % 8);  /* 8‑periodic pattern */
    return block[idx];
}
