/*
 * A039852  Number of partitions satisfying cn(0,5) + cn(1,5) + cn(4,5) <= 1.
 * https://oeis.org/A039852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p₂₃(n) + Σ_{s≡0,1,4 (mod5)} p₂₃(n‑s), where p₂₃(k) is the
 *            number of partitions of k using only parts congruent to 2 or 3
 *            modulo 5. The function computes the values by dynamic
 *            programming
 * Range:     u8 n <= 27, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
#include <stdlib.h>
#include <stddef.h>

static seq_t *dp0 = NULL;         /* partitions using parts 2,3 (mod5) */
static seq_t *a_arr = NULL;       /* final sequence values */
static idx_t   max_computed = 0;  /* maximum n for which arrays are filled */

/* Recompute dp0 and a_arr up to N (inclusive) */
static void recompute_upto(idx_t N)
{
    /* Allocate new arrays */
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * (N + 1));
    seq_t *a  = (seq_t *)malloc(sizeof(seq_t) * (N + 1));
    if (!dp || !a) {
        /* memory allocation failed */
        exit(1);
    }

    /* Initialize dp[0] = 1, rest 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= N; ++i) dp[i] = 0;

    /* Compute partitions using only parts ≡ 2 or 3 (mod 5) */
    for (idx_t part = 2; part <= N; ++part) {
        if (part % 5 == 2 || part % 5 == 3) {
            for (idx_t i = part; i <= N; ++i) {
                dp[i] += dp[i - part];
            }
        }
    }

    /* a[n] starts as dp[n] (no special part) */
    for (idx_t n = 1; n <= N; ++n) a[n] = dp[n];
    a[0] = 0; /* unused, but keep zero */

    /* Add contributions of exactly one special part (≡0,1,4 mod5) */
    for (idx_t s = 1; s <= N; ++s) {
        if (s % 5 == 0 || s % 5 == 1 || s % 5 == 4) {
            for (idx_t n = s; n <= N; ++n) {
                a[n] += dp[n - s];
            }
        }
    }

    /* Replace old arrays */
    free(dp0);
    free(a_arr);
    dp0 = dp;
    a_arr = a;
    max_computed = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */
    if (n > max_computed) recompute_upto(n);
    return a_arr[n];
}
