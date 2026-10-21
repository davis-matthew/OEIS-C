/*
 * A087298  Exponent of 2 in the prime factorization of (3n)!.
 * https://oeis.org/A087298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=1} floor(3n / 2^k)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)3 * n;  /* m = 3n */
    seq_t sum = 0;
    m >>= 1;                 /* start with floor(3n/2) */
    while (m > 0) {
        sum += m;
        m >>= 1;             /* go to floor(3n/4), floor(3n/8), ... */
    }
    return sum;
}
