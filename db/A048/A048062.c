/*
 * A048062  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{M} C(L, 2m) * C(n-L, m) where L = ⌊(n-4)/2⌋ and
 *            M = min(⌊n/3⌋, ⌊L/2⌋, n-L)
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 40, u64 n <= 75, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine uses only integer arithmetic, relying on the
   integer type definitions seq_t (result type) and idx_t (index type)
   supplied by the build system.  */

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use the smaller k for efficiency */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* At this point the division is exact.  */
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t L = (n > 4) ? (n - 4) / 2 : 0;
    idx_t nL = n - L;

    /* Compute M = min(floor(n/3), floor(L/2), n-L).  */
    idx_t M = n / 3;
    idx_t m2 = L / 2;
    if (m2 < M) M = m2;
    if (nL < M) M = nL;

    seq_t result = 0;
    for (idx_t m = 1; m <= M; ++m)
    {
        seq_t small = binom_idx(L, 2 * m);
        seq_t large = binom_idx(nL, m);
        result += small * large;
    }
    return result;
}
