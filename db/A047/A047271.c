/*
 * A047271  Numbers that are congruent to {0, 3, 5} mod 6.
 * https://oeis.org/A047271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k+1) = 6k, a(3k+2) = 6k+3, a(3k+3) = 6k+5
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Offset is 1: a(1) = 0, a(2) = 3, ...
    idx_t k = (n - 1) / 3;   // integer division
    idx_t r = (n - 1) % 3;   // remainder 0,1,2

    seq_t base = (seq_t)k * 6;   // 6k

    switch ((int)r) {
        case 0: return base;          // 6k
        case 1: return base + 3;      // 6k+3
        case 2: return base + 5;      // 6k+5
        default: return 0;            // unreachable
    }
}
