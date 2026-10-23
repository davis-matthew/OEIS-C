/*
 * A172330  Floor(n*(sqrt(13)+sqrt(11))).
 * https://oeis.org/A172330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor(n*(sqrt(13)+sqrt(11)))
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit unsigned integers.
   Uses binary search with 128‑bit arithmetic to avoid overflow. */
static inline unsigned long long isqrt_u64(unsigned long long x) {
    unsigned long long lo = 0, hi = 0xFFFFFFFFULL, ans = 0;
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    unsigned long long nn = (unsigned long long)n;

    /* Compute floor(n*sqrt(13)) and floor(n*sqrt(11)) via integer sqrt. */
    unsigned long long s1 = isqrt_u64(13ULL * nn * nn);
    unsigned long long s2 = isqrt_u64(11ULL * nn * nn);

    unsigned long long base = s1 + s2;          /* floor parts sum          */
    unsigned long long M    = base + 1;         /* candidate with +1       */

    /* Check whether floor((n+1)*α) = base + 1 or base. */
    unsigned long long lhs_cons = 24ULL * nn * nn;   /* 24 n²           */
    unsigned long long base2    = M * M;            /* (base+1)²       */

    if (base2 <= lhs_cons) {
        /* In this case M² ≤ 24n² → adjustment of +1 is guaranteed. */
        return (seq_t)(base + 1);
    } else {
        /* Need to test 4 n⁴ 143  ≥ (M² – 24n²)²  (no sqrt involved). */
        unsigned __int128 left  = (unsigned __int128)572 * (nn * nn) * (nn * nn); /* 572 = 4⋅143 */
        unsigned __int128 diff  = (unsigned __int128)base2 - lhs_cons;          /* M² – 24n²  (positive) */
        unsigned __int128 right = diff * diff;

        if (left >= right) {
            return (seq_t)(base + 1);
        } else {
            return (seq_t)base;
        }
    }
}
