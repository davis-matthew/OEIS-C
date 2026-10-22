/*
 * A002326  Multiplicative order of 2 mod 2n+1.
 * https://oeis.org/A002326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   multiplicative order of 2 mod (2n+1), i.e., least m > 0 such
 *            that 2n+1 divides 2^m - 1
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = (seq_t)n * 2 + 1;
    
    if (p == 1) return 1;
    
    seq_t power = 2 % p;
    seq_t m = 1;
    
    while (power != 1) {
        power = (power * 2) % p;
        m++;
    }
    
    return m;
}
