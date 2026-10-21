/*
 * A086754  Pascal's square pyramid read by slices, each slice being read by
 *          rows. Each entry in slice n is the sum of the 4 entries above it
 *          in slice n-1.
 * https://oeis.org/A086754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = binomial(m,i) * binomial(m,j) where m is the slice number
 *            (0‑based) such that S(m-1) < n ≤ S(m), with S(m) = ∑_{k=0}^{m}
 *            (k+1)^2 = (m+1)(m+2)(2m+3)/6. The offset within slice m
 *            (0‑based) is offset = n - S(m-1) - 1. Then i = offset / (m+1)
 *            and j = offset % (m+1)
 * Range:     u8 n <= 108, u16 n <= 547, u32 n <= 2619, u64 n <= 8689,
 *            u128 n <= 8689, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n - k)  /* use symmetry */
        k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);   /* multiply by numerator */
        res = res / (seq_t)i;              /* divide by denominator */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1 in OEIS */
        return 0;

    /* Find the slice number m such that S(m-1) < n ≤ S(m). */
    idx_t m = 0;
    __int128 total = 1;          /* S(0) = 1 */
    while ((__int128)n > total)
    {
        ++m;
        idx_t len = m + 1;        /* slice size is len × len */
        total += (__int128)len * len;   /* add slice capacity */
    }

    /* Number of elements before this slice. */
    idx_t len = m + 1;
    __int128 before = total - (__int128)len * len;   /* S(m-1) */

    /* Zero‑based offset inside slice m. */
    idx_t offset = (idx_t)((__int128)n - before - 1);

    idx_t i = offset / len;
    idx_t j = offset % len;

    seq_t ci = binom(m, i);
    seq_t cj = binom(m, j);

    return ci * cj;
}
