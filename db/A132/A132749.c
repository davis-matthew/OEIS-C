/*
 * A132749  Triangle T(n,k) = binomial(n, k) with T(n, 0) = 2, read by rows.
 * https://oeis.org/A132749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, less
 * Formula:   a(n) = first element of the (n+1)-term triangle with * T(0,0) =
 *            1, T(r,0) = 2 for r > 0, and T(r,k) = binomial(r,k) for k > 0 *
 *            read row‑wise
 * Range:     u8 n <= 69, u16 n <= 197, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/* Helper: compute binomial coefficient with overflow safe intermediate
 * For 64‑bit seq_t we use a 128‑bit accumulator.
 * For larger (or arbitrary‑precision) seq_t we rely on seq_t arithmetic
 * which offers / and * operators. */
static seq_t binom_u64(idx_t n, idx_t k)
{
    /* Reduce k */
    if (k > n - k)
        k = n - k;
    if (k == 0)
        return (seq_t)1;

#if defined(__SIZEOF_INT128__)
    /* For seq_t <= 64 bits use 128‑bit intermediate */
    if (sizeof(seq_t) <= 8) {
        __uint128_t res = 1;
        for (idx_t i = 1; i <= k; ++i) {
            __uint128_t ni = n - k + i;  /* numerator   */
            __uint128_t di = i;          /* denominator */
            res = res * ni / di;
        }
        return (seq_t)res;
    }
#endif
    /* Fall back to seq_t arithmetic (handles arbitrary precision) */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t ni = (seq_t)(n - k + i);
        seq_t di = (seq_t)i;
        res = (res * ni) / di;
    }
    return res;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    /* Grow the search interval until it exceeds n */
    while (((__uint128_t)high * (high + 1)) / 2 <= n) {
        if (high > ((idx_t)-1) >> 1)   /* avoid overflow on shift */
            break;
        high <<= 1;
    }
    /* Binary search for exact row */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__uint128_t)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    /* Column index within the row */
    idx_t start = (idx_t)((( __uint128_t)r * (r + 1)) / 2);
    idx_t k = n - start;

    /* Special left‑most element (except top row) */
    if (k == 0 && r > 0)
        return (seq_t)2;

    return binom_u64(r, k);
}
