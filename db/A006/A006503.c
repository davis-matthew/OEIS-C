/*
 * A006503  a(n) = n*(n+1)*(n+8)/6.
 * https://oeis.org/A006503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+8)/6
 * Range:     u8 n <= 9, u16 n <= 70, u32 n <= 2950, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t sn1 = sn + 1;
    seq_t sn8 = sn + 8;
    
    // Divide by 2
    if (sn % 2 == 0) {
        sn /= 2;
    } else {
        sn1 /= 2;
    }
    
    // Divide by 3
    if (sn % 3 == 0) {
        sn /= 3;
    } else if (sn1 % 3 == 0) {
        sn1 /= 3;
    } else {
        sn8 /= 3;
    }
    
    return sn * sn1 * sn8;
}
