/*
 * A182533  A symmetrical triangle. Read by rows: T(n,k) = 2*C(n-2,k-1) -
 *          C(n-2,k) - C(n-2,k-2), n >= 2, 0 <= k <= n, with T(0,0) = 0,
 *          T(1,0) = T(1,1) = 1.
 * https://oeis.org/A182533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r and k are obtained from the n-th entry of
 *            the triangle T(n,k) read by rows. For r ≥ 2 and 0 ≤ k ≤ r,
 *            T(r,k) = 2*C(r-2,k-1) – C(r-2,k) – C(r-2,k-2), with T(0,0)=0,
 *            T(1,0)=T(1,1)=1
 * Range:     i8 n <= 97, i16 n <= 264, i32 n <= 839, i64 n <= 2736,
 *            i128 n <= 5151, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binom128: compute binomial coefficient C(n,k) in 128‑bit integer */
static __int128 binom128(idx_t n, idx_t k)
{
    if (k > n) return ( __int128 )0;
    if (k == 0 || k == n) return ( __int128 )1;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ( (__int128 )(n - k + i) );
        res = res / ( (__int128 )i );
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n = 0, just safety */

    /* Find row r such that (r+1)(r+2)/2 >= n > r(r+1)/2 . */
    idx_t low = 0, high = n;            /* r <= n for n>=1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t total = ( (__uint128_t)(mid + 1) * (mid + 2) ) / 2;
        if (total >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of previous elements before row r */
    __uint128_t prev = 0;
    if (r > 0)
        prev = ( (__uint128_t)r * (r + 1) ) / 2;

    /* Column index within the row (0‑based) */
    idx_t k = (idx_t)(n - prev - 1);

    /* Special cases for tiny rows */
    if (r == 0) return (seq_t)0;        /* T(0,0) */
    if (r == 1) return (seq_t)1;        /* T(1,0)=T(1,1)=1 */

    /* Compute T(r,k) using the given formula, with 128‑bit intermediate. */
    __int128 term = 0;
    if (k >= 1)
        term += (__int128)2 * binom128(r - 2, k - 1);
    if (k <= r - 2)
        term -= binom128(r - 2, k);
    if (k >= 2)
        term -= binom128(r - 2, k - 2);

    return (seq_t)term;
}
