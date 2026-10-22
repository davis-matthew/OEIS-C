/*
 * A005586  a(n) = n*(n+4)*(n+5)/6.
 * https://oeis.org/A005586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+4)*(n+5)/6
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t y = x + 4;
    seq_t z = x + 5;
    
    if (y % 2 == 0) {
        y = y / 2;
    } else {
        z = z / 2;
    }
    
    if (x % 3 == 0) {
        x = x / 3;
    } else if (y % 3 == 0) {
        y = y / 3;
    } else {
        z = z / 3;
    }
    
    return x * y * z;
}
