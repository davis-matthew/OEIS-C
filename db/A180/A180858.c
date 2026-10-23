/*
 * A180858  Square array read by antidiagonals: T(p,n) is the Wiener index of
 *          the fan graph F(p,n) (p>=1, n>=1). F(p,n) is the graph obtained by
 *          placing an edge between each node of the empty graph on p nodes
 *          and each node of the path graph on n nodes.
 * https://oeis.org/A180858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = p(p-1)+(q-1)^2+p*q, where (p,q) are obtained by reading
 *            the array T(p,q) = p(p-1)+(q-1)^2+p*q along antidiagonals with
 *            decreasing p on each diagonal. Let t = s-1 be the smallest
 *            integer with t*(t+1)/2 >= n. Then prev = (t-1)*t/2, k = n - prev
 *            - 1 (0‑based offset), p = t - k, q = k + 1
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find t = s-1 : minimal t s.t. t*(t+1)/2 >= n */
    idx_t t = 0;
    while ((t * (t + 1ULL) / 2) < n)
        ++t;

    /* number of elements before this diagonal */
    idx_t prev = (t - 1ULL) * t / 2ULL;

    /* offset within the diagonal (0‑based) */
    idx_t k = n - prev - 1ULL;

    /* coordinates on the diagonal (p, q) */
    idx_t p = t - k;
    idx_t q = k + 1ULL;

    /* compute T(p,q) using seq_t arithmetic */
    const seq_t one = (seq_t)1;
    seq_t pp = (seq_t)p;
    seq_t qq = (seq_t)q;

    seq_t term = pp * (pp - one) + (qq - one) * (qq - one) + pp * qq;
    return term;
}
