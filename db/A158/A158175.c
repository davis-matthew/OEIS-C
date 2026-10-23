/*
 * A158175  Number of nondecreasing integer sequences of length 41 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 135, u128 n <= 204,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A158175 – Number of nondecreasing integer sequences of length 41
   with sum zero and sum of absolute values 2*n.
   Formula: a(n) = \sum_{k=1}^{41}\sum_{p=1}^{41-k} q_k(n) · q_p(n)
   where q_k(n) is the number of partitions of n into exactly k parts.
   For n ≤ 20, this reduces to a(n)=p(n)^2 (p = partition function). */
#include <stddef.h>
#include <stdlib.h>

/* Maximum number of parts allowed – the sequence length (41) */
#define K_MAX 41

/* Array index macro for the flat q_exact table */
static size_t idx(size_t n, size_t k)
{
    return n * (K_MAX + 1) + k;
}

/* Cached data */
static seq_t *q_exact = NULL;          /* flattened table q_exact[n][k] (0≤k≤K_MAX) */
static seq_t *a_cache = NULL;          /* a_cache[n] = a(n) for 0≤n≤cached_n */
static idx_t cached_n = 0;             /* highest n with computed value */
static idx_t alloc_n  = 0;             /* allocated size in the n‑dimension */

/* Ensure that values up to n are computed and cached */
static void ensure_cached(idx_t n)
{
    if (n <= cached_n) return;                 /* already computed */

    /* Grow allocation if necessary */
    if (n >= alloc_n) {
        idx_t new_alloc = (alloc_n > 0) ? alloc_n * 2 : n + 1;
        if (new_alloc <= n + 1) new_alloc = n + 1;

        /* Allocate larger tables */
        seq_t *new_q = (seq_t *)malloc(new_alloc * (K_MAX + 1) * sizeof(seq_t));
        seq_t *new_a = (seq_t *)malloc((new_alloc + 1) * sizeof(seq_t));
        if (!new_q || !new_a) return;          /* allocation failed – unlikely */

        /* Copy old data */
        if (q_exact && a_cache) {
            for (idx_t i = 0; i <= cached_n; ++i) {
                size_t base_src = i * (K_MAX + 1);
                size_t base_dst = i * (K_MAX + 1);
                for (int k = 0; k <= K_MAX; ++k)
                    new_q[base_dst + k] = q_exact[base_src + k];
            }
            memcpy(new_a, a_cache, (cached_n + 1) * sizeof(seq_t));
        }

        /* Free old memory */
        free(q_exact);
        free(a_cache);

        /* Install new tables */
        q_exact = new_q;
        a_cache = new_a;
        alloc_n = new_alloc;
    }

    /* Base case n = 0 */
    if (cached_n == 0 && q_exact) {
        q_exact[idx(0,0)] = (seq_t)1;
        for (int k = 1; k <= K_MAX; ++k) q_exact[idx(0,k)] = (seq_t)0;
        a_cache[0] = (seq_t)0;
    }

    /* Compute rows from cached_n+1 up to n */
    for (idx_t cur = cached_n + 1; cur <= n; ++cur) {
        /* q_exact[cur][k] – partitions of cur into exactly k parts */
        for (int k = 1; k <= K_MAX; ++k) {
            if ((idx_t)k <= cur) {
                seq_t part1 = q_exact[idx(cur - 1, k - 1)];
                seq_t part2 = q_exact[idx(cur - k, k)];
                q_exact[idx(cur, k)] = part1 + part2;
            } else {
                q_exact[idx(cur, k)] = (seq_t)0;
            }
        }

        /* Compute a(cur) using the convolution of the q_exact row */
        seq_t sum = (seq_t)0;
        for (int k = 1; k <= K_MAX; ++k) {
            seq_t qk = q_exact[idx(cur, k)];
            if (qk == (seq_t)0) continue;
            seq_t inner = (seq_t)0;
            int max_p = K_MAX - k;
            for (int p = 1; p <= max_p; ++p)
                inner += q_exact[idx(cur, p)];
            sum += qk * inner;
        }
        a_cache[cur] = sum;
    }

    cached_n = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 – return 0 for safety */
    ensure_cached(n);
    return a_cache[n];
}
