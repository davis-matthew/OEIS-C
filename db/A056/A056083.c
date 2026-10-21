/*
 * A056083  Numbers k such that k^6 == 1 (mod 7^3).
 * https://oeis.org/A056083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = residues[(n-1) mod 6] + 343 * ((n-1) / 6) with residues =
 *            {1,18,19,324,325,342}
 * Range:     u8 n <= 3, u16 n <= 1149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t residues[6] = {1,18,19,324,325,342};
    idx_t block = (n - 1) / 6;
    idx_t pos   = (n - 1) % 6;
    seq_t period = (seq_t)block * (seq_t)343;
    return residues[pos] + period;
}
