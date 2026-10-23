/*
 * A158459  Period 4: repeat [0, 3, 2, 1].
 * https://oeis.org/A158459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4 - (n mod 4)) mod 4 → 0,3,2,1 repeating
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use a simple table to avoid any arithmetic overflow issues */
    switch (n & 3U) {
        case 0: return 0;
        case 1: return 3;
        case 2: return 2;
        case 3: return 1;
    }
    return 0; /* unreachable */
}
