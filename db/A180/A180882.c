/*
 * A180882  Number of permutations p() of 1..n+5 with centered difference
 *          p(i+1)-p(i-1) < 0 exactly 4 times.
 * https://oeis.org/A180882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+5, ceil((n+5)/2)) * ∑_{d=0}^4 E(ceil((n+5)/2),
 *            d) * E(floor((n+5)/2), 4-d) where E(m,k) are Eulerian numbers
 *            (permutations of m with exactly k descents)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t); */

/* static storage for Eulerian numbers A(n,k) for k = 0..4; */
static seq_t *euler_arr = NULL;
static idx_t euler_max_n = 0;   /* largest n whose row is stored */

/* Ensure euler_arr contains rows up to max_n (inclusive). */
static void ensure_euler(idx_t max_n)
{
    if (max_n <= euler_max_n) return;

    size_t new_size = (size_t)(max_n + 1) * 5;
    seq_t *new_arr = (seq_t *)realloc(euler_arr, new_size * sizeof(seq_t));
    if (!new_arr) return; /* allocation failed – subsequent use will crash naturally */

    euler_arr = new_arr;

    for (idx_t n = euler_max_n + 1; n <= max_n; ++n) {
        /* initialise the new row */
        for (idx_t k = 0; k < 5; ++k) euler_arr[n * 5 + k] = 0;
        euler_arr[n * 5 + 0] = 1;               /* A(n,0) = 1 for all n>=1 */
        if (n == 1) continue;                  /* base case completed */
        for (idx_t k = 1; k < 5; ++k) {
            if (k > n - 1) continue;           /* A(n,k)=0 if k>=n */
            seq_t term1 = ((seq_t)(n - k)) * euler_arr[(n - 1) * 5 + (k - 1)];
            seq_t term2 = ((seq_t)(k + 1)) * euler_arr[(n - 1) * 5 + k];
            euler_arr[n * 5 + k] = term1 + term2;
        }
    }
    euler_max_n = max_n;
}

/* Safely retrieve Eulerian number A(n,k) for 0 ≤ k ≤ 4; 0 if out of range. */
static inline seq_t euler(idx_t n, idx_t k)
{
    if (k > 4 || n > euler_max_n || k > n - 1) return 0;
    return euler_arr[n * 5 + k];
}

/* External linkage function required by the interface. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for n<1 (offset 1)  */
    idx_t m = n + 5;                            /* total length of the permutation */
    idx_t L = (m + 1) / 2;                      /* number of odd positions  (ceil) */
    idx_t K = m / 2;                            /* number of even positions (floor) */

    idx_t needed = L > K ? L : K;
    ensure_euler(needed);

    seq_t sum = 0;
    for (idx_t d = 0; d <= 4; ++d) {
        if (d > L - 1 || (4 - d) > K - 1) continue;
        seq_t term = euler(L, d) * euler(K, 4 - d);
        sum += term;
    }

    seq_t bin = binomial(m, L);
    return bin * sum;
}
