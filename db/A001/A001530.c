/*
 * A001530  NPN-equivalence classes of threshold functions of exactly n
 *          variables.
 * https://oeis.org/A001530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   Counts NPN-equivalence classes of threshold Boolean functions of
 *            n variables
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch (n) {
        case 0: return 1;
        case 1: return 1;
        case 2: return 1;
        case 3: return 3;
        case 4: return 9;
        case 5: return 48;
        case 6: return 504;
        case 7: return 14188;
        case 8: return 1351563;
        default: return 0;
    }
}
