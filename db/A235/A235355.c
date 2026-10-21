/*
 * A235355  0 followed by the sum of (1),(2), (3,4),(5,6), (7,8,9),(10,11,12)
 *          from the natural numbers.
 * https://oeis.org/A235355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0. For odd n: let p=(n+1)/2, a(n) = (2*p^3 - p^2 + p)/2.
 *            For even n: let p=n/2, a(n) = (2*p^3 + p^2 + p)/2
 * Range:     u8 n <= 12, u16 n <= 80, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n & 1ULL) {                 /* odd n */
        seq_t p  = ((seq_t)n + 1) / 2;
        seq_t p2 = p * p;
        seq_t p3 = p2 * p;
        return (2 * p3 - p2 + p) / 2;
    } else {                        /* even n */
        seq_t p  = (seq_t)(n / 2);
        seq_t p2 = p * p;
        seq_t p3 = p2 * p;
        return (2 * p3 + p2 + p) / 2;
    }
}
