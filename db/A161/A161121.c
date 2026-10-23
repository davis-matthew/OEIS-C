/*
 * A161121  Triangle read by rows: T(n,k) is the number of fixed-point-free
 *          involutions of {1,2,...,2n} having k cycles with entries of the
 *          same parity (0 <= k <= 2*floor(n/2)).
 * https://oeis.org/A161121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term of the triangle T(r,k): * T(r,k) = (r-k)!
 *            * C(r,k)^2 * ((k-1)!!)^2 for even k, * T(r,k) = 0 for odd k. *
 *            The linear index n is converted to row r and column k
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 64, u64 n <= 167,
 *            u128 n <= 457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t N)
{
    /* Find the row r such that cumulative length > N. */
    idx_t r   = 0;
    idx_t cum = 0;
    while (1) {
        idx_t len = (r & 1) ? r : (r + 1);    /* L(r) = r+1 if even, r if odd */
        if (cum + len > N) {
            break;                           /* n lies in row r */
        }
        cum += len;
        ++r;
    }

    idx_t k = N - cum;        /* column within row */
    if (k & 1) return 0;      /* odd k gives zero */

    /* (r-k)! */
    seq_t fact = 1;
    for (idx_t v = 2; v <= r - k; ++v) {
        fact *= (seq_t)v;
    }

    /* binomial coefficient C(r, k) */
    idx_t kk = k;
    if (kk > r - kk) kk = r - kk;   /* use symmetry to reduce size */
    seq_t bin = 1;
    for (idx_t i = 1; i <= kk; ++i) {
        bin *= (seq_t)(r - kk + i);
        bin /= (seq_t)i;
    }

    /* (k-1)!! odd double factorial */
    seq_t df = 1;
    for (idx_t v = 1; v < k; v += 2) {
        df *= (seq_t)v;
    }

    /* Assemble the result */
    seq_t result = fact;
    result *= bin;
    result *= bin;          /* square the binomial */
    result *= df;
    result *= df;           /* square the double factorial */

    return result;
}
