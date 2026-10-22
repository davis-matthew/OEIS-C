/*
 * A001445  a(n) = (2^n + 2^[ n/2 ] )/2.
 * https://oeis.org/A001445
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 2^[n/2])/2 = 2^(n-1) + 2^([n/2]-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return ((seq_t)1 << (n - 1)) + ((seq_t)1 << (n / 2 - 1));
}
