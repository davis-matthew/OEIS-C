/*
 * A048840  Expansion of (1 - x + 2*x^2 + 2*x^3 - x^4 - x^5)/(1-x)^3.
 * https://oeis.org/A048840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n + 3*n - 6 for n >= 3 Initial values: a(0)=1, a(1)=2,
 *            a(2)=5
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 1;
    if (n == 1)
        return 2;
    if (n == 2)
        return 5;
    seq_t N = (seq_t)n;
    seq_t result = N * N + 3 * N;
    result -= 6;
    return result;
}
