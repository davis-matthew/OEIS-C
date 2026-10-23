/*
 * A129367  Triangle T(n, k) = A002415(n-k+3)*A002415(k+3), read by rows.
 * https://oeis.org/A129367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = A002415(n-k+3) * A002415(k+3), where A002415(t) =
 *            t^2*(t^2-1)/12 and the terms are flattened row‑by‑row: n =
 *            offset(r)+k, offset(r)=r*(r+1)/2, 0≤k≤r
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_uint128(__int128 v)
{
    /* floor(sqrt(v)) for non‑negative v, v fits in __int128. */
    idx_t lo = 0;
    idx_t hi = 1;
    /* Find an upper bound hi such that hi*hi > v. */
    while ((__int128)hi * hi <= v) {
        hi <<= 1;
    }
    /* Binary search in [lo, hi). */
    while (hi - lo > 1) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * mid <= v) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    __int128 m128 = (__int128)1 + (__int128)8 * n;
    idx_t s = isqrt_uint128(m128);
    idx_t r = (s - 1) / 2;

    /* Offset of the start of row r. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;              /* column index within the row */

    /* Compute t1 = r - k + 3, t2 = k + 3. */
    idx_t t1 = r - k + 3;
    idx_t t2 = k + 3;

    /* Convert to seq_t for arithmetic. */
    seq_t tt1 = (seq_t)t1;
    seq_t tt2 = (seq_t)t2;

    /* A002415(t) = t^2*(t^2-1)/12. */
    seq_t tt1_sq = tt1 * tt1;
    seq_t val1 = (tt1_sq * (tt1_sq - (seq_t)1)) / (seq_t)12;

    seq_t tt2_sq = tt2 * tt2;
    seq_t val2 = (tt2_sq * (tt2_sq - (seq_t)1)) / (seq_t)12;

    return val1 * val2;
}
