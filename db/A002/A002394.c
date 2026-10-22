/*
 * A002394  Weight distribution of [ 7,4,3 ] Hamming code.
 * https://oeis.org/A002394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weight distribution of [7,4,3] Hamming code; a(0)=1, a(3)=a(4)=7, a(7)=1, a(n)=0 otherwise */
seq_t a(idx_t n) {
    switch (n) {
        case 0: return 1;
        case 3: return 7;
        case 4: return 7;
        case 7: return 1;
        default: return 0;
    }
}
