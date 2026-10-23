/*
 * A189642  Numerator of H(n+4) - H(n), where H(n) = Sum_{k=1..n} 1/k.
 * https://oeis.org/A189642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator of
 *            (4*n^3+30*n^2+70*n+50)/((n+1)*(n+2)*(n+3)*(n+4))
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 262, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _seq_gcd(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;

    /* Terms n+1 … n+4 */
    seq_t n1 = N + (seq_t)1;
    seq_t n2 = N + (seq_t)2;
    seq_t n3 = N + (seq_t)3;
    seq_t n4 = N + (seq_t)4;

    /* Numerator polynomial 4*n^3 + 30*n^2 + 70*n + 50 */
    seq_t n2sq = N * N;
    seq_t n3cub = n2sq * N;
    seq_t num = 4 * n3cub + 30 * n2sq + 70 * N + (seq_t)50;

    /* Denominator product (n+1)(n+2)(n+3)(n+4) */
    seq_t denom = n1 * n2;
    denom = denom * n3;
    denom = denom * n4;

    /* Reduce fraction to lowest terms and return numerator */
    seq_t g = _seq_gcd(num, denom);
    return num / g;
}
