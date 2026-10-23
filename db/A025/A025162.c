/*
 * A025162  Number of partitions of n with distinct parts p(i) such that if i
 *          != j, then |p(i) - p(j)| >= 8.
 * https://oeis.org/A025162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k≥1} P(n – 4k(k–1), k) where P(s, k) is the number of
 *            partitions of s into exactly k parts (no restriction on
 *            distinctness). This follows from the bijection a_i = b_i +
 *            8(i–1) with b_1 ≤ b_2 ≤ … ≤ b_k, b_i ≥ 1
 * Range:     u8 n <= 67, u16 n <= 178, u32 n <= 534, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a DP table P[s][k] computed once up to
   the maximum n requested (at most 10000).  The table size is
   (nmax+1) × (kmax+1) with kmax ≈ 50.  The function is
   memoised with static storage and works correctly for any order of
   calls.  It is O(nmax·kmax) memory and time for the initial pass,
   and each subsequent call is O(kmax).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *P_table = NULL;   /* flat array P[s*(kmax+1)+k] */
static idx_t N_max = 0;         /* maximum n pre‑computed */
static size_t K_max = 0;        /* maximum k used */

/* initialise DP table up to at least max_n */
static void init_dp(idx_t max_n)
{
    if (N_max >= max_n) return;      /* already computed up to required n */

    /* compute new limits */
    N_max = max_n;

    /* find largest k with minimal sum 4k(k-1) ≤ N_max */
    K_max = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t min_sum = 4 * k * (k - 1);
        if (min_sum > N_max) break;
        K_max = k;
    }

    /* allocate table */
    size_t rows = (size_t)(N_max + 1);
    size_t cols = (size_t)(K_max + 1);
    size_t size = rows * cols;
    P_table = (seq_t *)malloc(size * sizeof(seq_t));
    if (!P_table) exit(EXIT_FAILURE);   /* allocation failure */

    /* initialise to zero */
    for (size_t i = 0; i < size; ++i) P_table[i] = 0;

    /* base case: P[0][0] = 1 */
    P_table[0 * cols + 0] = (seq_t)1;

    /* DP recurrence: P[s][k] = P[s-1][k-1] + P[s-k][k] (if s>=k) */
    for (idx_t s = 1; s <= N_max; ++s) {
        for (idx_t k = 1; k <= K_max && k <= s; ++k) {
            seq_t val = P_table[(s - 1) * cols + (k - 1)];
            if (s >= k) val += P_table[(s - k) * cols + k];
            P_table[s * cols + k] = val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 0, but safe */

    /* ensure DP table ready for this n */
    init_dp(n);

    size_t cols = K_max + 1;
    seq_t result = (seq_t)0;

    for (idx_t k = 1; k <= K_max; ++k) {
        /* S' = n - 4k(k-1) */
        idx_t shift = 4 * k * (k - 1);
        if (shift > n) break;
        idx_t s_prime = n - shift;
        if (s_prime < k) continue;  /* impossible to partition into k parts */

        result += P_table[s_prime * cols + k];
    }

    return result;
}
