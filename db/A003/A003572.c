/*
 * A003572  Order of 3 mod 3n+2.
 * https://oeis.org/A003572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 3 modulo (3n+2)
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)(3 * n + 2);
    seq_t pow3 = (seq_t)3 % m;
    seq_t k = (seq_t)1;
    
    while (pow3 != 1) {
        pow3 = (pow3 * (seq_t)3) % m;
        ++k;
    }
    
    return k;
}
