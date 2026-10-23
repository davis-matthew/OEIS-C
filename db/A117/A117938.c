/*
 * A117938  Triangle, columns generated from Lucas Polynomials.
 * https://oeis.org/A117938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = f_k(r-k+1) where r = smallest integer with r*(r+1)/2 >=
 *            n, k = n - r*(r-1)/2, f_1(x)=1, f_{j+1}(x)=x*f_j(x)+f_{j-1}(x)
 *            (Lucas polynomials)
 * Range:     u8 n <= 32, u16 n <= 73, u32 n <= 182, u64 n <= 453,
 *            u128 n <= 1160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used for the OEIS sequence */

    /* Find the row r such that n lies in it. */
    idx_t r = 1;
    idx_t t = 1;                    /* total terms up to row r */
    while (t < n) {
        ++r;
        t += r;
    }

    /* Number of terms before row r. */
    idx_t start_before = t - r;
    /* Column index within the row (1‑based). */
    idx_t k = n - start_before;
    if (k == 1) return (seq_t)1;   /* first column is constant 1 */

    /* Argument x for the Lucas polynomial. */
    idx_t x = r - k + 1;

    /* Compute Lucas polynomial L_{k-1}(x). */
    seq_t L0 = (seq_t)2;           /* L0 = 2 */
    seq_t L1 = (seq_t)x;           /* L1 = x */
    if (k == 2) return L1;

    seq_t Lprev = L1, Lcurr;
    for (idx_t i = 2; i <= k-1; ++i) {
        Lcurr = (seq_t)x * Lprev + L0;
        L0 = Lprev;
        Lprev = Lcurr;
    }
    return Lprev;
}
