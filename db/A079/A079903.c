/*
 * A079903  a(n) = (9n^4 - 18n^3 + 18n^2 - 9n + 2)/2.
 * https://oeis.org/A079903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (3*n*n - 3*n + 1)*(3*n*n - 3*n + 2)/2
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* offset not used, but defined for safety */
    seq_t nn = (seq_t)n;

    /* t1 = 3*n^2 - 3*n + 1 (always odd) */
    seq_t t1 = (seq_t)3 * nn * nn - (seq_t)3 * nn + (seq_t)1;

    /* t2 = t1 + 1 (always even) */
    seq_t t2 = t1 + (seq_t)1;

    return (t1 * t2) / 2;                  /* division is exact */
}
