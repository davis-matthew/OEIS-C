/*
 * A205392  Ordered differences of numbers s(j)=ceiling(j^2/2).
 * https://oeis.org/A205392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(i) – s(j) where s(k) = ceil(k²/2) = (k*k + 1)/2, and
 *            (i, j) are obtained from the n‑th entry in antidiagonal order of
 *            pairs i > j: i is the smallest integer with i(i–1)/2 ≥ n, j = n
 *            – (i–1)(i–2)/2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates floor(sqrt(x)) for 0 ≤ x < 2⁶⁷ using binary search on uint64_t. */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t low  = 0;
    uint64_t high = ((uint64_t)1 << 34);   /* sqrt(8*(2^64−1)+1) < 2^34 */

    while (low <= high) {
        uint64_t mid   = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* Find i such that i(i‑1)/2 >= n and (i‑1)(i‑2)/2 < n. */
    unsigned __int128 d  = (unsigned __int128)8 * n + 1;
    uint64_t       r     = isqrt_uint128(d);
    idx_t          i     = (r + 1) >> 1;  /* floor((1+sqrt)/2) */

    /* Adjust i if needed (at most once). */
    unsigned __int128 tri = (unsigned __int128)i * (i - 1) / 2;
    while (tri < n) {
        ++i;
        tri = (unsigned __int128)i * (i - 1) / 2;
    }

    /* j is the position within row i. */
    unsigned __int128 prev_tri = (unsigned __int128)(i - 1) * (i - 2) / 2;
    idx_t          j = n - prev_tri;   /* j ∈ [1, i‑1] */

    /* s(k) = ceil(k²/2) = (k*k + 1)/2 */
    seq_t s_i = ((seq_t)i * (seq_t)i + 1) / 2;
    seq_t s_j = ((seq_t)j * (seq_t)j + 1) / 2;

    return s_i - s_j;
}
