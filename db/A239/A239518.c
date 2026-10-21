/*
 * A239518  Number of partitions p of n that are separable by the number of
 *          parts of p; see Comments.
 * https://oeis.org/A239518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions p of n that are h‑separable with h =
 *            number of parts of p. A partition is counted when the part r
 *            (the number of parts) occurs m = floor(r/2) or m = ceil(r/2)
 *            times, and the remaining r−m parts contain no part equal to r
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values are pre‑computed for all n ≤ 20000.  The algorithm uses
   the recurrence p_k(m) = p_k(m−k) + p_{k−1}(m−1) for the number
   of partitions of m into exactly k parts (no restriction on part
   sizes).  For a given r and multiplicity m the number of admissible
   partitions equals
          R_k(S) = p_k(S) − p_{k−1}(S−r)    (S = n−r·m, k = r−m). */

#include <stdlib.h>
#include <stdbool.h>

static const idx_t N_MAX = 20000;          /* pre‑compute for n up to 20000 */

/* dimensions for the DP tables */
static idx_t MAX_R = 0;                    /* maximal number of parts needed   */
static idx_t MAX_K = 0;                    /* maximal k (=ceil(r/2))          */
static seq_t *dp = NULL;                   /* p_k(m) table (flat 2‑D)         */
static seq_t *a_table = NULL;              /* pre‑computed a(n) values        */

/* initialise tables only once */
static void init_tables(void)
{
    static bool ready = false;
    if (ready) return;

    /* determine maximal r and k for the chosen N_MAX */
    idx_t r = 0;
    for (; (r + 1) * (r + 1) <= 2 * N_MAX; ++r) { }
    MAX_R = r;
    MAX_K = (MAX_R + 1) / 2;        /* ceil(r/2) for the largest r */

    /* allocate flat DP table dp[k][m]  (k=0…MAX_K, m=0…N_MAX)      */
    size_t stride  = (size_t)(N_MAX + 1);
    size_t dp_size = (size_t)(MAX_K + 1) * stride;
    dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
    if (!dp) { exit(1); }

    /* allocate result table a(n)  (n=0…N_MAX)                       */
    a_table = (seq_t *)malloc((N_MAX + 1) * sizeof(seq_t));
    if (!a_table) { exit(1); }

    /* initialise tables to zero */
    for (size_t i = 0; i < dp_size; ++i) dp[i] = (seq_t)0;
    for (size_t i = 0; i <= N_MAX; ++i) a_table[i] = (seq_t)0;

    /* base case: p_0(0) = 1 */
    dp[0 + 0 * stride] = (seq_t)1;

    /* recurrence: p_k(m) = p_k(m - k) + p_{k-1}(m - 1) */
    for (idx_t k = 1; k <= MAX_K; ++k) {
        for (idx_t m = 1; m <= N_MAX; ++m) {
            seq_t val = (seq_t)0;
            if (m >= k) {
                val += dp[k * stride + (m - k)];
            }
            val += dp[(k - 1) * stride + (m - 1)];
            dp[k * stride + m] = val;
        }
    }

    /* compute a(n) for all n ≤ N_MAX */
    for (idx_t r = 2; r <= MAX_R; ++r) {
        if ((r & 1U) == 0) {                  /* r even: single m = r/2   */
            idx_t m = r >> 1;                 /* m = r/2                   */
            idx_t k = m;                      /* k = r-m = r/2             */
            idx_t start = r * m;              /* smallest n for this r     */

            for (idx_t n = start; n <= N_MAX; ++n) {
                idx_t S = n - r * m;
                seq_t add = dp[k * stride + S];
                if (S >= r) {
                    seq_t sub = dp[(k - 1) * stride + (S - r)];
                    add -= sub;
                }
                a_table[n] += add;
            }
        } else {                               /* r odd: two possibilities */
            /* first possibility: m = (r-1)/2 */
            idx_t m1 = (r - 1) >> 1;
            idx_t k1 = r - m1;              /* = (r+1)/2                */
            idx_t start1 = r * m1;

            for (idx_t n = start1; n <= N_MAX; ++n) {
                idx_t S = n - r * m1;
                seq_t add = dp[k1 * stride + S];
                if (S >= r) {
                    seq_t sub = dp[(k1 - 1) * stride + (S - r)];
                    add -= sub;
                }
                a_table[n] += add;
            }

            /* second possibility: m = (r+1)/2 */
            idx_t m2 = (r + 1) >> 1;
            idx_t k2 = r - m2;              /* = (r-1)/2                */
            idx_t start2 = r * m2;

            for (idx_t n = start2; n <= N_MAX; ++n) {
                idx_t S = n - r * m2;
                seq_t add = dp[k2 * stride + S];
                if (S >= r) {
                    seq_t sub = dp[(k2 - 1) * stride + (S - r)];
                    add -= sub;
                }
                a_table[n] += add;
            }
        }
    }

    ready = true;
}

seq_t a(idx_t n)
{
    init_tables();
    if (n > N_MAX) return (seq_t)0;
    return a_table[n];
}
