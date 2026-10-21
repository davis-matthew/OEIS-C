/*
 * A240057  Number of partitions of n such that (greatest part) is not =
 *          (multiplicity of greatest part).
 * https://oeis.org/A240057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p(n) - Σ_{g≥1, g²≤n} p_{g-1}(n - g²)
 * where p(n)   = number of partitions of n,
 *       p_k(m) = number of partitions of m with all parts ≤ k.
 * The function below maintains static tables for p(n) and p_k(m)
 * up to the maximum n ever requested, recomputed when a larger n
 * is requested.  The tables are built with the standard
 * partition‑DP (only additions), so no negative intermediate
 * values appear and it works for any unsigned seq_t (including
 * arbitrary‑precision types).
 */
#include <stdlib.h>
#include <string.h>

static idx_t max_precomp_N = 0;
static seq_t *partitions = NULL;              /* p(n) */
static seq_t **pk_arr      = NULL;            /* pk_arr[k] = array of p_k(m) */
static idx_t max_k_comp    = 0;               /* largest k computed for pk_arr */

/* integer square root for idx_t values (n ≤ 10000 in typical use) */
static idx_t isqrt(idx_t n) {
    idx_t t = 0;
    while ((t + 1) * (t + 1) <= n) ++t;
    return t;
}

/* Rebuild all tables for all values up to N (inclusive).  Called only when
 * a new, larger n is requested.  The rebuild is O(N²) but suffices for N≈10 000. */
static void rebuild_tables(idx_t N) {
    /* free previous pk_arr */
    if (pk_arr) {
        for (idx_t k = 0; k <= max_k_comp; ++k)
            free(pk_arr[k]);
        free(pk_arr);
    }

    /* allocate and initialize partitions array */
    partitions = (seq_t *)realloc(partitions, (N + 1) * sizeof(seq_t));
    memset(partitions, 0, (N + 1) * sizeof(seq_t));
    partitions[0] = 1;
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t sum = part; sum <= N; ++sum) {
            partitions[sum] += partitions[sum - part];
        }
    }

    /* allocate pk_arr up to needed k = floor(sqrt(N)) - 1 */
    idx_t kmax = isqrt(N);
    if (kmax > 0) --kmax;          /* largest k such that k+1 ≤ sqrt(N) */
    pk_arr = (seq_t **)malloc((kmax + 1) * sizeof(seq_t *));
    max_k_comp = kmax;

    for (idx_t k = 0; k <= kmax; ++k) {
        pk_arr[k] = (seq_t *)malloc((N + 1) * sizeof(seq_t));
        memset(pk_arr[k], 0, (N + 1) * sizeof(seq_t));
        pk_arr[k][0] = 1;          /* empty partition */
        if (k == 0) continue;      /* p_0(m>0)=0 */

        for (idx_t part = 1; part <= k; ++part) {
            for (idx_t sum = part; sum <= N; ++sum) {
                pk_arr[k][sum] += pk_arr[k][sum - part];
            }
        }
    }

    max_precomp_N = N;
}

/* public function required by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined below offset, but harmless */

    /* ensure tables cover at least n */
    if (n > max_precomp_N)
        rebuild_tables(n);

    seq_t total       = partitions[n];
    seq_t forbidden   = 0;
    for (idx_t g = 1; g * g <= n; ++g) {
        idx_t r   = n - g * g;
        seq_t val = pk_arr[g - 1][r];   /* p_{g-1}(r) */
        forbidden += val;
    }

    return total - forbidden;
}
