/*
 * A208375  Number of n X 4 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*n^2 + 40*n - 60 for n>1; a(1) = 10
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1u) return (seq_t)10;
    seq_t sn = (seq_t)n;
    return (seq_t)20 * sn * sn + (seq_t)40 * sn - (seq_t)60;
}
