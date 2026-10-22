/*
 * A006513  Limit of the image of n after 2k iterates of `(3x+1)/2' map as k
 *          grows.
 * https://oeis.org/A006513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count iterations to reach 1; return 1 if even, 2 if odd
 * Range:     u8 n <= 26, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    idx_t count = 0;
    
    while (x != 1) {
        if (x & 1) {  // x is odd
            x = (3 * x + 1) / 2;
        } else {  // x is even
            x = x / 2;
        }
        count++;
    }
    
    return (count & 1) ? 2 : 1;
}
