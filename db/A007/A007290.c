/*
 * A007290  a(n) = 2*binomial(n,3).
 * https://oeis.org/A007290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(n,3) = n*(n-1)*(n-2)/3
 * Range:     u8 n <= 10, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if (sn < 3) return 0;
    
    if (sn % 3 == 0) {
        return (sn / 3) * (sn - 1) * (sn - 2);
    } else if ((sn - 1) % 3 == 0) {
        return sn * ((sn - 1) / 3) * (sn - 2);
    } else {
        return sn * (sn - 1) * ((sn - 2) / 3);
    }
}
