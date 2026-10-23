/*
 * A175485  Numerators of averages of squares of the first n positive
 *          integers.
 * https://oeis.org/A175485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n+1)*(2*n+1)*gcd(6,n)/6
 * Range:     u8 n <= 10, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t n_ = (seq_t)n;
    seq_t n1 = n_ + 1;
    seq_t n2 = n_ * 2 + 1;
    seq_t prod = n1 * n2;
    idx_t g = 1;
    if ((n & 1) == 0) g *= 2;   /* factor 2 if n even */
    if (n % 3 == 0) g *= 3;      /* factor 3 if n divisible by 3 */
    return (prod * (seq_t)g) / 6;
}
