/*
 * A143062  Expansion of false theta series variation of Euler's pentagonal
 *          number series in powers of x.
 * https://oeis.org/A143062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n = k(3k+1)/2, -1 if n = k(3k-1)/2, otherwise 0 (k ≥
 *            0)
 * Range:     i8 n <= 391, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* k = 0 gives value 1 */

    for (idx_t k = 1;; ++k) {
        seq_t ks = (seq_t)k;
        seq_t t1 = ks * (3 * ks + 1) / 2;   /* k(3k+1)/2   */
        seq_t t2 = ks * (3 * ks - 1) / 2;   /* k(3k-1)/2   */

        if (t1 > n && t2 > n) break;       /* no further matches possible */

        if (t1 == (seq_t)n) return 1;
        if (t2 == (seq_t)n) return -1;
    }
    return 0;
}
