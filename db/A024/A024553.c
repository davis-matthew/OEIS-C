/*
 * A024553  [ 1/{n*sqrt(5)} ], where {x} := x - [ x ].
 * https://oeis.org/A024553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ 1 / { n·√5 } ⌋ ( {x} = x – ⌊x⌋ )
 * Range:     u8 n <= 232, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root of an unsigned 64‑bit value.
   Returns ⌊√x⌋. */
static inline idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; n=0 never requested */

    /* 5 * n² fits in 64 bits for all n used in the tests (n ≤ 10 000). */
    idx_t n_sq = (idx_t)n * (idx_t)n;
    idx_t val  = 5 * n_sq;          /* = 5·n²  = (n·√5)²  */

    /* b = ⌊√(5·n²)⌋ = ⌊n·√5⌋ */
    idx_t b = isqrt_idx(val);

    /* e = 5·n² – b² = (n·√5 – b)(n·√5 + b)   (positive, small) */
    idx_t e = val - b * b;

    /* a(n) = ⌊ 2·b / e ⌋  (derived from 1/{n·√5}) */
    idx_t numerator = (idx_t)2 * b;
    idx_t result    = numerator / e;

    return (seq_t)result;
}
