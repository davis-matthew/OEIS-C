/*
 * A093479  Number of regular (infinite) apeirotopes of full rank in
 *          n-dimensional space.
 * https://oeis.org/A093479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(1)=1; a(2)=6; a(3)=8; a(4)=18; a(n)=8 for n>4
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 6;
    if (n == 3) return 8;
    if (n == 4) return 18;
    return 8;
}
