/*
 * A048085  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{⌊min(⌊(n+3)/3⌋, ⌊(n-⌊(n+3)/3⌋)/3⌋)⌋}
 *            C(⌊(n+3)/3⌋, m) * C(n-⌊(n+3)/3⌋, 3m) where C(x, y)=0 if y>x or
 *            y<0. This counts non‑empty subsets of {1,…,n} in which exactly
 *            one quarter of the chosen elements are ≤⌊(n+3)/3⌋. The summation
 *            index m is the number of elements taken from the lowe...
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* \*\/
 * Pre‑condition: seq_t and idx_t are supplied by the build system.
 * No external libraries are required.
 */

/* Helper: multiplicative binomial coefficient using seq_t arithmetic.
 * Returns C(n,k).  If k>n it returns 0.
 */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* For n < 5 the sequence is 0 (verified by definition). */
    if (n < 5) return (seq_t)0;

    seq_t res = (seq_t)0;
    idx_t t = (n + 3) / 3;          /* number of elements <= (n+3)/3 */
    idx_t remaining = n - t;        /* elements > (n+3)/3 */

    /* m cannot exceed t nor can 3m exceed remaining. */
    idx_t max_m = t;
    if (remaining / 3 < max_m) max_m = remaining / 3;

    for (idx_t m = 1; m <= max_m; ++m)
    {
        seq_t c1 = _binom(t, m);               /* choose m from first part */
        seq_t c2 = _binom(remaining, 3 * m);   /* choose 3m from second part */
        res += c1 * c2;
    }
    return res;
}
