/*
 * A022530  Nexus numbers (n+1)^14 - n^14.
 * https://oeis.org/A022530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^14 - n^14
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 24, u128 n <= 750,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 988 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow14_seq(seq_t base) {
    seq_t result = 1;
    for (int i = 0; i < 14; ++i) result *= base;
    return result;
}

seq_t a(idx_t n) {
    seq_t sn   = (seq_t)n;
    seq_t sn1  = sn + 1;
    seq_t p1   = pow14_seq(sn1);
    seq_t p2   = pow14_seq(sn);
    return p1 - p2;
}
