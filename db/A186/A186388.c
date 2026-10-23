/*
 * A186388  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=6*i and g(j)=j*(j+1)/2 (triangular
 *          number). Complement of A186387.
 * https://oeis.org/A186388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*(n+1)/12)
 * Range:     u8 n <= 15, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index to seq_t for computations */
    seq_t nn = (seq_t)n;

    /* Compute floor(n*(n+1)/12) */
    seq_t tmp = nn * (nn + 1);   /* n*(n+1) */
    seq_t part = tmp / 12;       /* integer division truncates */

    return nn + part;            /* a(n) = n + floor(n*(n+1)/12) */
}
