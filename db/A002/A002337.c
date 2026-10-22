/*
 * A002337  Weight distribution of [8,4,4] Hamming code.
 * https://oeis.org/A002337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Weight distribution of [8,4,4] Hamming code; a(0)=1, a(4)=14,
 *            a(8)=1, a(n)=0 otherwise
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch (n) {
        case 0: return 1;
        case 4: return 14;
        case 8: return 1;
        default: return 0;
    }
}
