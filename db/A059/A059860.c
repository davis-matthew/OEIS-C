/*
 * A059860  a(n) = binomial(n+1, 2)^5.
 * https://oeis.org/A059860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+1, 2)^5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 118, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The sequence is defined only for n ≥ 1.  For n = 0 we return 0
       to avoid undefined behaviour in the build. */
    if (n == 0) return 0;

    /* Compute the triangular number T(n) = n*(n+1)/2.  All operations
       are performed with the user‑defined integer type seq_t. */
    seq_t t = (seq_t)n * (seq_t)(n + 1) / 2;

    /* Raise T(n) to the 5th power by repeated multiplication. */
    seq_t r = t;
    for (int i = 0; i < 4; ++i) r *= t;

    return r;
}
