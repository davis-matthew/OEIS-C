/*
 * A004163  Pentagonal numbers written backwards.
 * https://oeis.org/A004163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000326(n)), where A000326(n) = n*(3*n-1)/2 and
 *            A004086 reverses digits
 * Range:     u8 n <= 8, u16 n <= 88, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t pent = sn * (3 * sn - 1) / 2;
    
    seq_t reversed = 0;
    seq_t temp = pent;
    
    while (temp > 0) {
        reversed = reversed * 10 + (temp % 10);
        temp /= 10;
    }
    
    return reversed;
}
