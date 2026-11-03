/*
 * A182074  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2+x*y=n.
 * https://oeis.org/A182074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of ordered pairs (x,y)∈[‑n,n]² such that x*y = ±k
   (k is given as a non‑negative absolute value).  */
static inline seq_t count_xy_for_k(idx_t n, uint64_t absK)
{
    if (absK == 0) {
        /* For k = 0 we have 2n+1 choices for each coordinate
           but double count (0,0).  */
        return (seq_t)4 * (seq_t)n + (seq_t)1;
    }

    seq_t cnt = 0;
    for (uint64_t d = 1; d <= absK / d; ++d) {   /* d ≤ sqrt(absK) */
        if (absK % d == 0) {
            uint64_t e = absK / d;
            if (d <= n && e <= n) {
                if (d == e) {
                    cnt += (seq_t)2;            /* (d,d) and (-d,-d) */
                } else {
                    cnt += (seq_t)4;            /* (±d,±e) and (±e,±d) */
                }
            }
        }
    }
    return cnt;
}
/* A182074: number of ordered triples (w,x,y) with w^2 + x*y = n and
   w,x,y ∈ {‑n,…,0,…,n}.  The value is computed by counting, for each
   possible w, all pairs (x,y) whose product equals n – w².  */

/* Main sequence function.  An argument of 0 does not correspond to the
   OEIS offset 1, but the function returns 0 in that case for safety. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* outside the sequence */

    seq_t ans = 0;

    /* w = 0 case */
    ans += count_xy_for_k(n, (uint64_t)n);

    /* w > 0, handle both +w and –w */
    for (idx_t w = 1; w <= n; ++w) {
        int64_t k = (int64_t)n - (int64_t)w * (int64_t)w;
        uint64_t absK = (k >= 0) ? (uint64_t)k : (uint64_t)(-k);
        seq_t cnt = count_xy_for_k(n, absK);
        ans += cnt * (seq_t)2;                /* count ±w */
    }

    return ans;
}
