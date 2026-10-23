/*
 * A193399  Wiener index of a benzenoid consisting of a chain of n hexagons
 *          characterized by the encoding s = 1133 (see the Gutman et al.
 *          reference, Sec. 5).
 * https://oeis.org/A193399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + 16*n^2 + 8*n + 2*(-1)^n*(n - 2) - 3
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = 4 * N * N * N + 16 * N * N + 8 * N;

    /* handle the alternating part 2*(-1)^n*(n-2) */
    int64_t sign = (n & 1) ? -1LL : 1LL;          /* (-1)^n */
    int64_t diff = (int64_t)n - 2LL;              /* n-2 (may be negative) */
    int64_t temp = sign * 2LL * diff;             /* 2*(-1)^n*(n-2) */

    if (temp >= 0) {
        term += (seq_t)temp;
    } else {
        term -= (seq_t)(-temp);
    }

    term -= (seq_t)3;  /* the final -3 in the formula */
    return term;
}
