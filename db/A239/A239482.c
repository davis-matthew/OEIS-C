/*
 * A239482  Number of (2,0)-separable partitions of n; see Comments.
 * https://oeis.org/A239482
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A239482: Number of (2,0)-separable partitions of n
   A partition is (2,0)-separable if the part 2 occurs exactly m times,
   the total number of parts is 2m+1, and the other parts are all ≠ 2.
   Hence

   a(n) = Σ_{m≥1} P_{m+1}^{(≠2)}(n - 2m),

   where P_{k}^{(≠2)}(s) is the number of partitions of s into exactly
   k parts with no part equal to 2.
*/

#include <stdlib.h>
#include <stdbool.h>

static const idx_t MAX_N = 10000;          /* maximum n to pre‑compute   */
static seq_t *dp_all_prev = NULL, *dp_all_cur = NULL;
static seq_t *dp_no2_prev = NULL, *dp_no2_cur = NULL;
static seq_t *a_arr        = NULL;
static bool   initialized  = false;

/* Convenience function to apply the recurrence for unrestricted partitions */
static void compute_makeup(idx_t N, idx_t k)
{
    idx_t n;
    /* compute p(n,k) with recurrence p(n,k) = p(n-1,k-1) + p(n-k,k)   */
    for (n = 0; n <= N; ++n) {
        seq_t val = 0;
        if (n >= 1) val += dp_all_prev[n-1];
        if (n >= k) val += dp_all_cur[n-k];
        dp_all_cur[n] = val;
    }
}

/* Main precomputation routine */
static void init(void)
{
    if (initialized) return;

    /* allocate arrays */
    dp_all_prev = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));
    dp_all_cur  = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));
    dp_no2_prev = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));
    dp_no2_cur  = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));
    a_arr       = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));

    /* initial conditions for unrestricted partitions (k = 0) */
    dp_all_prev[0] = 1;
    /* initial conditions for partitions with no part = 2 (k = 0) */
    dp_no2_prev[0] = 1;

    /* maximum m such that 3m+1 ≤ MAX_N  ->  m_max = floor((MAX_N-1)/3)   */
    const idx_t m_max = (MAX_N - 1) / 3;
    const idx_t k_max = m_max + 1;           /* k = m+1 */

    /* k = 1 is needed as base for k = 2, but produces no contribution to a(n) */
    for (idx_t k = 1; k <= k_max; ++k) {

        /* compute unrestricted partitions for this k */
        compute_makeup(MAX_N, k);

        /* compute partitions with no part equal to 2 for this k   */
        for (idx_t n = 0; n <= MAX_N; ++n) {
            seq_t val = 0;
            if (n >= 1) val += dp_no2_prev[n-1];
            if (n >= 2*k) val += dp_all_cur[n - 2*k];
            dp_no2_cur[n] = val;
        }

        /* accumulate contributions to a(n) for k ≥ 2 (i.e., m ≥ 1) */
        if (k >= 2) {
            const idx_t m = k - 1;
            const idx_t maxS = MAX_N - 2*m;
            for (idx_t S = 0; S <= maxS; ++S) {
                a_arr[S + 2*m] += dp_no2_cur[S];
            }
        }

        /* prepare for next iteration */
        seq_t *tmp = dp_all_prev; dp_all_prev = dp_all_cur; dp_all_cur = tmp;
        tmp = dp_no2_prev; dp_no2_prev = dp_no2_cur; dp_no2_cur = tmp;
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the platform                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n > MAX_N) return 0;          /* n larger than pre‑computed range   */
    return a_arr[n];
}
