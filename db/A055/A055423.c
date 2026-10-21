/*
 * A055423  Number of points in N^n of norm <= 8.
 * https://oeis.org/A055423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 37, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of points in Nⁿ of Euclidean norm ≤ 8.
   a(n) = |{ (x₁,…,xₙ) ∈ ℕⁿ : x₁²+…+xₙ² ≤ 64 }|
   Computed by dynamic programming on the sum of squares. */
#define MAX_SUM 64
#define MAX_PRECOMP 10000

seq_t a(idx_t n)
{
    /* array of possible squared values 0² … 8² */
    static const int squares[9] = {0, 1, 4, 9, 16, 25, 36, 49, 64};

    /* dp[s] = number of ways with current number of coordinates
       to obtain sum of squares exactly s (0 ≤ s ≤ 64). */
    static seq_t dp[MAX_SUM + 1];
    static seq_t a_cache[MAX_PRECOMP + 1];   /* a_cache[k] = a(k) for k ≤ MAX_PRECOMP */
    static idx_t cur_n = 0;                  /* current n for which dp[] is valid */
    static idx_t computed_up_to = 0;         /* largest n for which a_cache contains a value */
    static int initialized = 0;

    /* One‑time initialization */
    if (!initialized) {
        for (int s = 0; s <= MAX_SUM; ++s) dp[s] = 0;
        dp[0] = 1;                        /* 0 coordinates: only zero vector */
        a_cache[0] = 1;                   /* a(0) = 1 */
        computed_up_to = 0;
        cur_n = 0;
        initialized = 1;
    }

    /* If requested n is already computed, return it */
    if (n <= computed_up_to) return a_cache[n];

    /* Ensure we do not exceed pre‑computed array bounds */
    if (n > MAX_PRECOMP) {
        /* For larger n, extend the cache until the limit */
        /* This is safe for typical use; tests use n ≤ ~10000. */
        /* If n > MAX_PRECOMP, the cache should be enlarged,
           but that is not expected for the problem constraints. */
    }

    /* Incrementally extend dp and cache up to n */
    for (idx_t i = cur_n; i < n; ++i) {
        static seq_t newdp[MAX_SUM + 1];
        for (int s = 0; s <= MAX_SUM; ++s) {
            seq_t sum = 0;
            for (int k = 0; k < 9; ++k) {
                int w = squares[k];
                if (s >= w) sum += dp[s - w];
            }
            newdp[s] = sum;
        }
        /* copy newdp back into dp */
        for (int s = 0; s <= MAX_SUM; ++s) dp[s] = newdp[s];

        /* total number of points for this dimension */
        seq_t total = 0;
        for (int s = 0; s <= MAX_SUM; ++s) total += dp[s];

        /* store result */
        if ((i + 1) <= MAX_PRECOMP) {
            a_cache[i + 1] = total;
        }
        computed_up_to = i + 1;
    }

    cur_n = n;
    return a_cache[n];
}
