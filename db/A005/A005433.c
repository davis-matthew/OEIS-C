/*
 * A005433  A finite sequence associated with the Lie algebra B_3.
 * https://oeis.org/A005433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Finite sequence of dimensions associated with Lie algebra B_3
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 4;
    if (n == 4) return 10;
    if (n == 5) return 28;
    if (n == 6) return 120;
    if (n == 7) return 1680;
    return 0;
}
