/*
 * A035652  Number of partitions of n into parts 7k and 7k+2 with at least one
 *          part of each type.
 * https://oeis.org/A035652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n into parts 7k (k≥1) and 7k+2
 *            (k≥0) * with at least one part of each type. * Computed as a(n)
 *            = P(n) – P7(n) – P2(n) * where P(n) = partitions of n using
 *            parts ≡0 or 2 (mod 7), * P7(n) = partitions of n using only
 *            parts ≡0 (mod 7), * P2(n) = partitions of n using o...
 * Range:     u8 n <= 66, u16 n <= 161, u32 n <= 459, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp_total = NULL;   /* partitions with parts ≡0 or 2 mod 7 */
static seq_t *dp_7     = NULL;   /* partitions with parts ≡0 mod 7 only   */
static seq_t *dp_2     = NULL;   /* partitions with parts ≡2 mod 7 only   */
static size_t cached = 0;        /* number of elements currently cached (≥0) */

/* Resize the DP tables to hold values up to max_n (inclusive).  The
 * function recomputes all values from scratch, which is O(max_n^2)
 * but is executed only when a larger n is requested.  This keeps
 * per‑call time well below the 2 s limit for n≤10000. */
static void recompute(size_t max_n)
{
    size_t i, j, part;

    /* free previous tables if they exist */
    if (dp_total) free(dp_total);
    if (dp_7)     free(dp_7);
    if (dp_2)     free(dp_2);

    /* allocate new tables; use malloc so that they are zero‑initialized */
    dp_total = (seq_t*)calloc(max_n + 1, sizeof(seq_t));
    dp_7     = (seq_t*)calloc(max_n + 1, sizeof(seq_t));
    dp_2     = (seq_t*)calloc(max_n + 1, sizeof(seq_t));
    if (!dp_total || !dp_7 || !dp_2) { /* memory failure */
        exit(1);
    }

    /* initialize dp[0] = 1 for each kind of partition */
    dp_total[0] = 1;
    dp_7[0]     = 1;
    dp_2[0]     = 1;

    /* --------------------------------------------------------------------
     * 1) Partitions using parts ≡0 (mod 7) only
     * -------------------------------------------------------------------- */
    for (part = 7; part <= max_n; part += 7) {
        for (j = part; j <= max_n; ++j) {
            dp_7[j] += dp_7[j - part];
        }
    }

    /* --------------------------------------------------------------------
     * 2) Partitions using parts ≡2 (mod 7) only
     * -------------------------------------------------------------------- */
    for (part = 2; part <= max_n; part += 7) {
        for (j = part; j <= max_n; ++j) {
            dp_2[j] += dp_2[j - part];
        }
    }

    /* --------------------------------------------------------------------
     * 3) Partitions using parts ≡0 or 2 (mod 7)
     *    The generating function is the product of the two previous
     *    series, and the standard coin‑change DP applies.
     * -------------------------------------------------------------------- */
    for (part = 7; part <= max_n; part += 7) {
        for (j = part; j <= max_n; ++j) {
            dp_total[j] += dp_total[j - part];
        }
    }
    for (part = 2; part <= max_n; part += 7) {
        for (j = part; j <= max_n; ++j) {
            dp_total[j] += dp_total[j - part];
        }
    }

    cached = max_n;
}

seq_t a(idx_t n)
{
    /* Ensure we have computed up to n */
    if ((size_t)n > cached) {
        recompute((size_t)n);
    }

    /* a(n) = P(n) – P7(n) – P2(n)  for n>=1;   P(0)=1, P7(0)=1, P2(0)=1,
     * so for n=0 the result would be -1 which is never requested.     */
    seq_t res = dp_total[(size_t)n];
    res -= dp_7[(size_t)n];
    res -= dp_2[(size_t)n];
    return res;
}
