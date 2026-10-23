/*
 * A158163  Number of nondecreasing integer sequences of length 29 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{29} P(n,k) * Σ_{p=0}^{29-k} P(n,p) where P(n,k)
 *            is the number of partitions of the integer n into exactly k
 *            (positive) parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 44, u64 n <= 155, u128 n <= 238,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define KMAX 29   /* maximum number of parts that can appear in the sequence */

static idx_t dp_max = 0;     /* largest n for which dp has been computed */
static seq_t *dp = NULL;     /* flattened 2‑D table: dp[n*(KMAX+1)+k] = P(n,k) */

/* Ensure that dp[n] is available for all n up to *new_max*.
   The table is expanded only forwards. */
static void
ensure_dp(idx_t new_max)
{
    if (new_max <= dp_max)
        return;

    /* new memory size */
    size_t new_rows = (size_t)(new_max + 1);
    size_t new_len  = new_rows * (KMAX + 1);
    size_t old_len  = (size_t)(dp_max + 1) * (KMAX + 1);

    /* allocate / reallocate */
    seq_t *tmp;
    if (dp == NULL) {
        tmp = (seq_t *)calloc(new_len, sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
    }
    if (tmp == NULL)
        return;               /* allocation failed; the caller will get 0 */
    dp = tmp;

    /* zero the newly added memory */
    if (new_len > old_len)
        memset(dp + old_len, 0, (new_len - old_len) * sizeof(seq_t));

    /* initialise base row n = 0 (already zeroed except dp[0][0]) */
    dp[0] = 1;                 /* P(0,0) = 1 */

    /* compute rows for i = 1 .. new_max */
    for (size_t i = 1; i <= new_max; ++i) {
        size_t base_i = i * (KMAX + 1);
        for (size_t k = 1; k <= KMAX; ++k) {
            if (k > i) {
                /* P(i,k) = 0 when k > i */
                dp[base_i + k] = 0;
                continue;
            }
            /* recurrence: P(i,k) = P(i-1,k-1) + P(i-k,k) */
            size_t idx1 = (i - 1) * (KMAX + 1) + (k - 1);
            size_t idx2 = (i - k) * (KMAX + 1) + k;
            dp[base_i + k] = dp[idx1] + dp[idx2];
        }
    }

    dp_max = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* a(0) is undefined, return 0 */

    /* make sure we have partition numbers up to n */
    ensure_dp(n);

    /* read the row for n into a local array */
    seq_t part[KMAX + 1];
    size_t base_n = (size_t)n * (KMAX + 1);
    for (size_t k = 0; k <= KMAX; ++k)
        part[k] = dp[base_n + k];

    /* cumulative sums: C[t] = Σ_{p=0}^{t} part[p] */
    seq_t cum[KMAX + 1];
    cum[0] = part[0];
    for (size_t t = 1; t <= KMAX; ++t)
        cum[t] = cum[t - 1] + part[t];

    /* compute a(n) = Σ_{k=0}^{KMAX} part[k] * cum[KMAX - k] */
    seq_t result = 0;
    for (size_t k = 0; k <= KMAX; ++k) {
        seq_t pk = part[k];
        if (pk == 0)
            continue;
        seq_t sum_pk = cum[KMAX - k];
        result += pk * sum_pk;
    }

    return result;
}
