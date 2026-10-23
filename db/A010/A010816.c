/*
 * A010816  Expansion of Product_{k>=1} (1 - x^k)^3.
 * https://oeis.org/A010816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = n is triangular number t_m? (−1)^m*(2*m+1) : 0
 * Range:     i8 n <= 2, i16 n <= 54, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = 0           for n ≠ m(m+1)/2
 *   a(t_m) = (−1)^m*(2*m+1)
 *   m = floor((√(1+8n)−1)/2) when 1+8n is a perfect square
 */
static seq_t isqrt(seq_t v) {
    seq_t lo = 0;
    seq_t hi = v;
    while (lo <= hi) {
        seq_t mid = (lo + hi) / 2;
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    /* compute discriminant 1 + 8*n */
    seq_t disc = 8 * nseq + 1;
    seq_t s = isqrt(disc);
    /* check for perfect square */
    if (s * s != disc) return 0;

    /* m = (s - 1) / 2 */
    seq_t m = (s - 1) / 2;
    /* verify triangular */
    seq_t tri = m * (m + 1) / 2;
    if (tri != nseq) return 0;

    /* sign = (−1)^m */
    seq_t sign = ((m & 1) == 0) ? 1 : -1;
    return sign * (seq_t)(2 * m + 1);
}
