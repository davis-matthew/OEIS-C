/*
 * A152846  Triangle read by rows, A007318 rows repeated eight times .
 * https://oeis.org/A152846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = binomial(r, k) * where r = floor((sqrt(n+1)-1)/2), * k =
 *            (n - 4*r*(r+1)) mod (r+1). * The row r of A007318 is repeated
 *            eight times
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Safe 64‑bit integer square root (floor). */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t lo = 0, hi = 1;
    /* Find upper bound hi with hi*hi > n. */
    while ((unsigned __int128)hi * hi <= n)
        hi <<= 1;
    /* Binary search in [lo, hi). */
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Determine the row index r. */
    idx_t t = isqrt_u64(n + 1);   /* floor(sqrt(n+1)) */
    idx_t r = (t - 1) / 2;        /* floor((sqrt(n+1)-1)/2) */

    /* Total terms up to the previous row: S_{r-1} = 4*r*(r+1) */
    unsigned __int128 prev128 = (unsigned __int128)4 * r * (r + 1);
    idx_t prev_total = (idx_t)prev128;

    /* Position inside the current row block (0‑based). */
    idx_t offset = n - prev_total;
    idx_t k = offset % (r + 1);

    /* Compute binomial coefficient binom(r, k) multiplicatively. */
    idx_t kk = k;
    if (kk > r - kk)
        kk = r - kk; /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= kk; ++i)
    {
        res *= (seq_t)(r - kk + i);
        res /= (seq_t)i; /* division is exact */
    }
    return res;
}
