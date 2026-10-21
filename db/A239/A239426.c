/*
 * A239426  a(n) = 21*n^4 - 36*n^3 + 25*n^2 - 8*n + 1.
 * https://oeis.org/A239426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 120, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A239426: a(n) = 21*n^4 - 36*n^3 + 25*n^2 - 8*n + 1
   Equivalent factorization: a(n) = (1 - 3*n + 3*n^2) * (1 - 5*n + 7*n^2) */
seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;        /* Convert index to seq_t */
    seq_t n2 = nn * nn;         /* n^2 */
    /* Compute the two positive quadratic factors using only non‑negative arithmetic */
    seq_t f = 3 * n2 - 3 * nn + 1;   /* 1 - 3*n + 3*n^2  = 3*n^2 - 3*n + 1 */
    seq_t g = 7 * n2 - 5 * nn + 1;   /* 1 - 5*n + 7*n^2  = 7*n^2 - 5*n + 1 */
    return f * g;                  /* a(n) = f * g */
}
