/*
 * A224647  Number of (n+2)X6 0..1 matrices with each 3X3 subblock having the
 *          same population.
 * https://oeis.org/A224647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224647: Number of (n+2)×6 0..1 matrices with each 3×3
   subblock having the same population.
   Computation is performed as follows:
   - For n ≤ 20 the values are obtained by a DP over rows that
     enforces the subblock constraints directly from the definition.
   - For n > 20 the sequence satisfies the empirical linear recurrence
     (order 20) given in the OEIS entry:
        a(n) = 4·a(n-1) – 3·a(n-2) + 36·a(n-3) – 144·a(n-4) + 108·a(n-5) –
               503·a(n-6) + 2012·a(n-7) – 1509·a(n-8) + 3480·a(n-9) –
               13920·a(n-10) + 10440·a(n-11) – 12564·a(n-12) +
               50256·a(n-13) – 37692·a(n-14) + 22464·a(n-15) –
               89856·a(n-16) + 67392·a(n-17) – 15552·a(n-18) +
               62208·a(n-19) – 46656·a(n-20)      (for n > 20).
   The initial terms (n=1…20) are obtained from the definition by
   exploring all 6‑bit row patterns and enforcing the constraints
   on every 3×3 subblock.  These DP values are reused for the
   recurrence.  The function works for any order of calls and for arbitrary
   n ≥ 1.  It remains well under two seconds for n up to about 10 000
   (the recurrence is used for larger n). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Precomputed coefficients of the recurrence (index 0 ↔ a(n-1), … , 19 ↔ a(n-20)). */
static const int coeffs[20] = {
    4, -3, 36, -144, 108, -503, 2012, -1509, 3480, -13920,
    10440, -12564, 50256, -37692, 22464, -89856, 67392, -15552, 62208, -46656
};

/* Base values a(1)…a(20), computed from the definition. */
static seq_t base[21];
static int base_computed = 0;

/* Helper to compute base values using a DP that emulates the definition. */
static void compute_base_terms(void)
{
    if (base_computed) return;

    /* DP arrays:
       dp[p0][p1][p] holds the number of partial matrices that
       have the last two rows equal to p0, p1 and whose common
       3×3 subblock sum is p (0..9) or 10 when the common sum has
       not yet been fixed. */
    static seq_t dp[64][64][11];
    static seq_t newdp[64][64][11];
    memset(dp, 0, sizeof(dp));

    /* Initialise for the first two rows: common sum still undefined
       (represented by index 10).  All 64×64 pairs of rows are allowed. */
    for (int p0 = 0; p0 < 64; ++p0)
        for (int p1 = 0; p1 < 64; ++p1)
            dp[p0][p1][10] = 1;

    /* Helper for 3‑bit wide column masks: mask[j] covers columns j,j+1,j+2 (0‑based). */
    unsigned int masks[4];
    for (int j = 0; j < 4; ++j)
        masks[j] = ((1u << 3) - 1) << j;

    /* Build rows from 3 up to 22 (since n+2 rows are needed for n up to 20). */
    for (idx_t rows = 3; rows <= 22; ++rows) {
        memset(newdp, 0, sizeof(newdp));

        if (rows == 3) { /* first time we fix the common sum */
            for (int p0 = 0; p0 < 64; ++p0) {
                for (int p1 = 0; p1 < 64; ++p1) {
                    seq_t cnt = dp[p0][p1][10];
                    if (cnt == 0) continue;
                    for (int p2 = 0; p2 < 64; ++p2) {
                        int sums[4];
                        for (int j = 0; j < 4; ++j) {
                            unsigned int m = masks[j];
                            sums[j] = __builtin_popcount((unsigned)(p0 & m)) +
                                      __builtin_popcount((unsigned)(p1 & m)) +
                                      __builtin_popcount((unsigned)(p2 & m));
                        }
                        if (sums[0] == sums[1] && sums[1] == sums[2] && sums[2] == sums[3])
                            newdp[p1][p2][sums[0]] += cnt;
                    }
                }
            }
        } else { /* subsequent rows: the common sum is fixed (0..9) */
            for (int p0 = 0; p0 < 64; ++p0) {
                for (int p1 = 0; p1 < 64; ++p1) {
                    for (int p = 0; p < 10; ++p) {
                        seq_t cnt = dp[p0][p1][p];
                        if (cnt == 0) continue;
                        for (int p2 = 0; p2 < 64; ++p2) {
                            int ok = 1;
                            for (int j = 0; j < 4; ++j) {
                                unsigned int m = masks[j];
                                int sum = __builtin_popcount((unsigned)(p0 & m)) +
                                          __builtin_popcount((unsigned)(p1 & m)) +
                                          __builtin_popcount((unsigned)(p2 & m));
                                if (sum != p) { ok = 0; break; }
                            }
                            if (ok) newdp[p1][p2][p] += cnt;
                        }
                    }
                }
            }
        }

        /* Copy newdp back into dp for the next iteration. */
        memcpy(dp, newdp, sizeof(dp));

        /* Number of rows built so far is 'rows'.  Corresponding n is rows-2. */
        idx_t n = rows - 2;
        if (n <= 20) {
            seq_t total = 0;
            for (int p0 = 0; p0 < 64; ++p0)
                for (int p1 = 0; p1 < 64; ++p1)
                    for (int p = 0; p < 10; ++p)
                        total += dp[p0][p1][p];
            base[n] = total;
        }
    }
    base_computed = 1;
}

/* Storage for values a(k) for all k up to the largest value requested. */
static seq_t *cache = NULL;      /* dynamic array holding values[1..] */
static size_t cache_size = 0;    /* allocated size (index 0 unused) */
static idx_t cache_last = 0;     /* largest n for which cache[n] is valid */

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* value below offset: behaviour undefined */

    /* Ensure base terms are available. */
    if (!base_computed) compute_base_terms();

    if (n <= 20) return base[n];

    /* Initialise cache lazily. */
    if (cache == NULL) {
        cache_size = 21;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        for (size_t i = 0; i < cache_size; ++i) cache[i] = 0;
        for (size_t i = 1; i <= 20; ++i) cache[i] = base[i];
        cache_last = 20;
    }

    /* Grow cache if necessary. */
    if ((idx_t)cache_size <= n) {
        size_t new_cap = (size_t)(n + 1) * 2;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        for (size_t i = cache_size; i < new_cap; ++i) cache[i] = 0;
        cache_size = new_cap;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (int i = 0; i < 20; ++i) {
            int c = coeffs[i];
            if (c > 0)
                sum += (seq_t)c * cache[k - 1 - i];
            else
                sum -= (seq_t)(-c) * cache[k - 1 - i];
        }
        cache[k] = sum;
    }
    cache_last = n;
    return cache[n];
}
