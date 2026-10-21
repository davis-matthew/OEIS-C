/*
 * A064788  Inverse permutation to A060736.
 * https://oeis.org/A064788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (i + j – 1) * (i + j – 2) / 2 + j, where i = min(t, t² –
 *            n + 1), j = min(t, n – (t–1)²), t = floor(sqrt(n–1)) + 1
 *            (indexes start at 1)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t int_sqrt_u64(idx_t x)
{
    /* Integer square‑root by binary search, works for 0 ≤ x < 2^64. */
    idx_t lo = 0, hi = (idx_t)1 << 32;         /* 2^32 ≥ sqrt(2^64−1) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;        /* ceil(mid) */
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS definition; guard against 0 just in case. */
    if (n == 0) return 0;

    idx_t t = int_sqrt_u64(n - 1) + 1;          /* t ≥ 1 */

    idx_t prev   = t - 1;
    idx_t prev_sq = prev * prev;                /* (t‑1)² ≤ 2^64‑1 safely */

    idx_t i_expr = prev_sq + 2 * t - n;         /* t² – n + 1 rewritten */
    idx_t j_expr = n - prev_sq;                /* n – (t‑1)²            */

    idx_t i = (i_expr > t) ? t : i_expr;       /* min with t */
    idx_t j = (j_expr > t) ? t : j_expr;       /* min with t */

    seq_t sum = (seq_t)i + (seq_t)j;
    seq_t num = (sum - 1) * (sum - 2);
    seq_t result = num / 2 + (seq_t)j;
    return result;
}
