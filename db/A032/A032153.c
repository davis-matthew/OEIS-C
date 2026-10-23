/*
 * A032153  Number of ways to partition n elements into pie slices of
 *          different sizes.
 * https://oeis.org/A032153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=1..floor((sqrt(8*n+1)-1)/2)} (k-1)! * A008289(n,k)
 *            where A008289(n,k) = number of partitions of n into exactly k
 *            distinct parts. This implementation uses the identity
 *            A008289(n,k) = P_exact(n - k(k-1)/2 , k) with P_exact(m,k) the
 *            number of partitions of m into exactly k (not necessa...
 * Range:     u8 n <= 18, u16 n <= 37, u32 n <= 85, u64 n <= 206,
 *            u128 n <= 542, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#define MAX_K 141  /* sufficient for n <= 10000 (floor((sqrt(8*10000+1)-1)/2)=140) */

static seq_t *dp = NULL;          /* flat array for P_exact, size (rows)*(MAX_K+1) */
static seq_t *fact = NULL;        /* factorials (k-1)! up to MAX_K-1 */
static size_t current_max_n = 0;   /* highest n for which dp rows are computed */

/* ---- utility ----------------------------------------------------------- */
static idx_t kmax_for_n(idx_t n)
{
    idx_t k = 0;
    while (k * (k + 1) / 2 <= n) k++;
    return k - 1;   /* floor((sqrt(8n+1)-1)/2) */
}

static void ensure_tables(idx_t n)
{
    if (n <= current_max_n) return;          /* already computed */

    /* enlarge dp array to cover rows 0 .. n */
    size_t new_rows = (size_t)n + 1;
    size_t new_size = new_rows * (MAX_K + 1);
    seq_t *new_dp = (seq_t*)calloc(new_size, sizeof(seq_t));
    if (!new_dp) return;                     /* out‑of‑memory – undefined behaviour */

    /* copy old data if any */
    if (dp) {
        for (size_t r = 0; r <= current_max_n; ++r) {
            memcpy(new_dp + r * (MAX_K + 1),
                   dp + r * (MAX_K + 1),
                   (MAX_K + 1) * sizeof(seq_t));
        }
        free(dp);
    }
    dp = new_dp;

    /* set base case */
    dp[0 * (MAX_K + 1) + 0] = (seq_t)1;

    /* compute factorials once */
    if (!fact) {
        fact = (seq_t*)malloc((MAX_K + 1) * sizeof(seq_t));
        if (!fact) return;
        fact[0] = (seq_t)1;
        for (size_t i = 1; i <= MAX_K; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* compute new rows of dp */
    for (size_t row = current_max_n + 1; row <= (size_t)n; ++row) {
        for (size_t k = 1; k <= MAX_K; ++k) {
            seq_t val = dp[(row - 1) * (MAX_K + 1) + (k - 1)];
            if (row >= k) val += dp[(row - k) * (MAX_K + 1) + k];
            dp[row * (MAX_K + 1) + k] = val;
        }
    }

    current_max_n = (size_t)n;
}

/* ---- public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_tables(n);

    idx_t kmax_n = kmax_for_n(n);
    seq_t result = (seq_t)0;

    for (idx_t k = 1; k <= kmax_n; ++k) {
        idx_t mprime = n - k * (k - 1) / 2;
        seq_t part = dp[mprime * (MAX_K + 1) + k];
        seq_t term = fact[k - 1] * part;
        result += term;
    }
    return result;
}
