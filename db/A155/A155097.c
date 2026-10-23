/*
 * A155097  Numbers k such that k^2 == -1 (mod 37).
 * https://oeis.org/A155097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(-1)^(n+1) + 37*floor(n/2)
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n ≥ 1; behaviour for n < 1 is undefined */
    seq_t half = ((seq_t)n >> 1);            /* floor(n/2) */
    seq_t base = 37 * half;                  /* 37 * floor(n/2) */
    return (n & 1) ? base + 6 : base - 6;    /* odd: +6, even: -6 */
}
