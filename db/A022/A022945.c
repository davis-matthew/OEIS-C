/*
 * A022945  Duplicate of A022443.
 * https://oeis.org/A022945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = T(n) + extra(n) where T(n) = n*(n+1)/2 (triangular
 *            number) extra(n) = 1 + Σ_{t=2}^{T-1} t*(t+2) + (n - (T^2+3T-4)/2
 *            + 1)*T with T = floor((sqrt(8n+25) - 3)/2) (T ≥ 1)
 * Range:     i8 n <= 13, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 128‑bit values */
static uint64_t isqrt128(unsigned __int128 x)
{
    uint64_t low = 0, high = ~0ULL;        /* max uint64_t */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                           /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence is defined for n ≥ 1 */
    if (n == 1) return 1;

    /* Triangular part */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Compute T = floor((sqrt(8n+25) - 3)/2) */
    unsigned __int128 val = (unsigned __int128)n * 8 + 25;
    uint64_t sq    = isqrt128(val);
    uint64_t T_idx = (sq - 3) / 2;          /* T fits into idx_t (uint64_t) */

    seq_t sumt = 0;

    if (T_idx == 1) {                     /* only n = 2 falls here */
        sumt = 1;                         /* extra contribution for a(2) */
    } else {                              /* T_idx ≥ 2 */
        sumt = 1;                         /* contribution of t = 2 at k = 2 */

        /* Full plateaus t = 2 .. T-1 */
        for (idx_t t = 2; t < T_idx; ++t) {
            sumt += (seq_t)t * ((seq_t)t + 2);
        }

        /* Remaining part of the last plateau t = T */
        unsigned __int128 num = (unsigned __int128)T_idx * T_idx
                               + 3 * (unsigned __int128)T_idx
                               - 4;
        idx_t start_T = (idx_t)(num / 2);  /* first n belonging to plateau T */

        idx_t rem = n - start_T + 1;       /* how many terms of this plateau are used */
        sumt += (seq_t)rem * (seq_t)T_idx;
    }

    return tri + sumt;
}
