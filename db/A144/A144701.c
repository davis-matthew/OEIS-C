/*
 * A144701  Hankel transform of expansion of 1/c(x)^3, c(x) the g.f. of
 *          A000108.
 * https://oeis.org/A144701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((6 - 7*n - 9*n^2 - 2*n^3) / 6) if n≡0 (mod 3) * -(n +
 *            2)^2 if n≡1 (mod 3) * ((36 + 62*n + 30*n^2 + 4*n^3) / 12) if n≡2
 *            (mod 3)
 * Range:     i8 n <= 1, i16 n <= 19, i32 n <= 835, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A144701. */
seq_t a(idx_t n) {
    /* Convert to seq_t for arithmetic. */
    seq_t sn = (seq_t)n;
    seq_t nmod = n % 3;

    /* Pre‑compute n^2 and n^3. */
    seq_t n2 = sn * sn;         /* n^2 */
    seq_t n3 = n2 * sn;         /* n^3 */

    if (nmod == 0) {
        /* (6 - 7n - 9n^2 - 2n^3) / 6 */
        seq_t val = (seq_t)6 - (seq_t)7 * sn - (seq_t)9 * n2 - (seq_t)2 * n3;
        return val / 6;
    } else if (nmod == 1) {
        /* -(n + 2)^2 */
        seq_t t = sn + (seq_t)2;
        return - (t * t);
    } else { /* nmod == 2 */
        /* (36 + 62n + 30n^2 + 4n^3) / 12 */
        seq_t num = (seq_t)36 + (seq_t)62 * sn + (seq_t)30 * n2 + (seq_t)4 * n3;
        return num / 12;
    }
}
