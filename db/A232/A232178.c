/*
 * A232178  Least k>=0 such that triangular(n) + k^2 is a square, or -1 if no
 *          such k exists.
 * https://oeis.org/A232178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k >= 0 such that n*(n+1)/2 + k^2 is a perfect
 *            square; -1 if no such k
 * Range:     i8 n <= 15, i16 n <= 1230, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* triangular number T = n*(n+1)/2 */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* special case: T = 0 gives k = 0 immediately */
    if (T == 0)
        return 0;

    /* search over all divisor pairs (a,b) with a*b = T and a <= b */
    seq_t min_k = -1;  /* sentinel for “no solution” */

    for (seq_t a_div = 1; a_div <= T / a_div; ++a_div) {
        if (T % a_div != 0)
            continue;
        seq_t b = T / a_div;

        /* a and b must have the same parity for k to be integer */
        if (((a_div ^ b) & 1) != 0)
            continue;

        /* k = (b - a)/2  with b >= a */
        seq_t k = (b - a_div) >> 1;

        if (min_k == -1 || k < min_k)
            min_k = k;
    }

    return min_k;
}
