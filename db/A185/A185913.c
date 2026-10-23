/*
 * A185913  Accumulation array of A185912, by antidiagonals.
 * https://oeis.org/A185913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185913: Antidiagonal enumeration of A185912
 *
 * The underlying table is T(n,k) = C(k+1,2)*C(n+1,2)*(n^2+3*n+2*k)/6
 *   for n>=1, k>=1.
 *
 * A(n) is obtained by scanning the infinite lower‑triangular array
 *   in antidiagonal order:
 *     for s = 2,3,4,...
 *         for k = s-1, s-2, … , 1
 *             output T(n = s-k, k)
 *
 * This function computes A(n) directly from the index n (1‑based).
 * It works with the generic integer types seq_t and idx_t provided
 * by the build environment.  No temporary buffers or global state
 * are used, so calls in any order are safe.
 */

seq_t a(idx_t pos)
{
    /* Find the diagonal s (sum n+k) that contains the element
     * at position pos.
     * Number of elements up to diagonal s is (s-1)*s/2.
     */
    idx_t s = 1;
    idx_t tri = 0;
    while (tri < pos) {
        ++s;
        tri = (s - 1) * s / 2;
    }

    /* Determine offset within this diagonal (0‑based). */
    idx_t prev = (s - 2) * (s - 1) / 2;          /* elements before current diag */
    idx_t offset = pos - prev - 1;               /* 0‑based index within diagonal */

    idx_t k = s - 1 - offset;   /* k decreases along the diagonal as required */
    idx_t n = s - k;            /* n = s - k */

    /* Compute T(n,k) using the compact formula:
     * T = (k+1)*k*(n+1)*n*(n^2+3*n+2*k) / 24
     */
    seq_t sk = (seq_t)k;
    seq_t nk = (seq_t)n;
    seq_t num = (sk + 1) * sk;
    num *= (nk + 1);
    num *= nk;
    seq_t part = nk * nk + 3 * nk + 2 * sk;
    num *= part;
    seq_t result = num / (seq_t)24;

    return result;
}
