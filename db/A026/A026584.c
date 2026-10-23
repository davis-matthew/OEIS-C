/*
 * A026584  Irregular triangular array T read by rows: T(i,0) = T(i,2i) = 1
 *          for i >= 0; T(i,1) = T(i,2i-1) = floor(i/2) for i >= 1; and for i
 *          >= 2 and j = 2..2i-2, T(i,j) = T(i-1,j-2) + T(i-1,j-1) + T(i-1,j)
 *          if i+j is odd, and T(i,j) = T(i-1,j-2) + T(i-1,j) if i+j is even.
 * https://oeis.org/A026584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th entry of the irregular triangular array T read
 *            left‑to‑right. * T(i,0) = T(i,2i) = 1, * T(i,1) = T(i,2i-1) =
 *            floor(i/2) (i ≥ 1), * for 2 ≤ j ≤ 2i−2: * if (i + j) even:
 *            T(i,j) = T(i‑1,j‑2) + T(i‑1,j) * else: T(i,j) = T(i‑1,j‑2) +
 *            T(i‑1,j‑1) + T(i‑1,j). * a(1) = T(0,0) = 1; a(n) fo...
 * Range:     u8 n <= 71, u16 n <= 237, u32 n <= 751, u64 n <= 2645,
 *            u128 n <= 9695, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1U) return (seq_t)0; /* undefined for n < 1, return 0 */

    /* convert to zero‑based index */
    idx_t m = n - 1U;

    /* find row r such that r² ≤ m < (r+1)²  (cumulative entries up to row r is (r+1)²) */
    idx_t r = 0U;
    while ((r + 1U) * (r + 1U) <= m) ++r;

    /* position within row r (0‑based) */
    idx_t j = m - r * r;

    /* row 0 contains only a single 1 */
    if (r == 0U) return (seq_t)1;

    /* arrays large enough for n up to about 10 000 (row length ≤ 199) */
    seq_t prev[210];
    seq_t cur [210];

    /* initialize row 0 */
    prev[0] = (seq_t)1;

    for (idx_t i = 1U; i <= r; ++i)
    {
        idx_t cur_len = 2U * i + 1U;

        /* left and right boundaries */
        cur[0]                 = (seq_t)1;
        cur[cur_len - 1U]      = (seq_t)1;

        /* second and second‑last elements */
        seq_t half = (seq_t)(i / 2U);
        cur[1]                = half;
        cur[cur_len - 2U]     = half;

        /* interior elements */
        for (idx_t k = 2U; k <= cur_len - 3U; ++k)  /* k runs 2 .. 2i-2 */
        {
            if (((i + k) & 1U) == 0U)
                cur[k] = prev[k - 2U] + prev[k];
            else
                cur[k] = prev[k - 2U] + prev[k - 1U] + prev[k];
        }

        if (i == r) /* reached target row */
            return cur[j];

        /* copy current row into previous row for next iteration */
        for (idx_t t = 0U; t < cur_len; ++t)
            prev[t] = cur[t];
    }

    /* should never reach here */
    return (seq_t)0;
}
