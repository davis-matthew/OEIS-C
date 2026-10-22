/*
 * A001082  Generalized octagonal numbers: k*(3*k-2), k=0, +- 1, +- 2, +-3,
 *          ...
 * https://oeis.org/A001082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - n - floor(n/2)^2
 * Range:     u8 n <= 18, u16 n <= 296, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t half_n = nn >> 1;
    return nn * nn - nn - half_n * half_n;
}
