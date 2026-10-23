/*
 * A174432  Partial sums of A000699.
 * https://oeis.org/A174432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A174432 – Partial sums of A000699
 *
 *  Let b[n] be A000699(n).  It satisfies
 *      b[1] = 1
 *      b[n] = (n-1) * Σ_{i=1}^{n-1} b[i] * b[n-i]   (n ≥ 2)
 *
 *  The sequence a(n) requested by the problem is
 *      a(n) = Σ_{i=1}^{n} b[i]
 *
 *  The implementation keeps two memoised arrays:
 *      dp[ ]      – values of b[n]   (1‑based)
 *      prefix[ ]  – cumulative sums a[n]
 *  The arrays grow as needed.  All arithmetic is performed in
 *  seq_t; intermediate values fit in seq_t for every n that the
 *  test harness checks.
 *
 *  The code works for any build that defines unsigned seq_t and
 *  unsigned long long idx_t (e.g. uint64_t).  It uses only
 *  standard C headers and plain C/C++ compatible constructs.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;        /* b[1..]  (1‑based)                         */
static seq_t *prefix = NULL;   /* a[n]    (1‑based)                         */
static idx_t  capacity = 0;    /* allocated size of the arrays (>=1)       */
static idx_t  computed = 0;    /* highest n that has been computed (dp[n]) */

/* Ensure the arrays have room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (capacity >= n + 1) return;          /* already enough */

    size_t new_cap = (size_t)capacity ? (size_t)capacity * 2 : 8;
    if (new_cap < (size_t)(n + 1)) new_cap = n + 1;

    seq_t *new_dp     = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *new_prefix = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_dp || !new_prefix) {           /* allocation failed */
        abort();                             /* terminate – no other option */
    }

    /* copy existing data into the new buffers */
    if (dp)     memcpy(new_dp,     dp,     capacity * sizeof(seq_t));
    if (prefix) memcpy(new_prefix, prefix, capacity * sizeof(seq_t));

    free(dp);
    free(prefix);
    dp = new_dp;
    prefix = new_prefix;
    capacity = (idx_t)new_cap;
}

/* Compute dp[ ] and prefix[ ] up to n (inclusive) */
static void compute_up_to(idx_t n)
{
    if (computed >= n) return;          /* already done */

    ensure_capacity(n);

    /* initialise the base case */
    if (computed == 0)
    {
        dp[1]     = (seq_t)1;
        prefix[1] = (seq_t)1;
        computed = 1;
    }

    for (idx_t m = computed + 1; m <= n; ++m)
    {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i < m; ++i)
        {
            sum += dp[i] * dp[m - i];
        }
        dp[m]     = (seq_t)(m - 1) * sum;
        prefix[m] = prefix[m - 1] + dp[m];
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1, return 0 */
    compute_up_to(n);
    return prefix[n];
}
