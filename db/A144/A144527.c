/*
 * A144527  a(n) = A000055(n) - 2.
 * https://oeis.org/A144527
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144527: a(n) = A000055(n) – 2.
   a(n) gives the number of free trees with n nodes, each node of degree ≤ n–3.
   We compute the rooted-tree counts R[n] and use the classical relation
   between rooted and unrooted trees:   A(n) = R(n) – ½∑_{i=1}^{n-1}R(i)R(n‑i)
   + (½)·[n even]·R(n/2).  The function returns A(n) – 2.  The implementation
   is memoised with static arrays that grow lazily.  All arithmetic is done
   with type seq_t.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *R      = NULL;   /* rooted-tree counts */
static seq_t *F      = NULL;   /* free-tree counts (A000055) */
static seq_t *sigma  = NULL;   /* σ(n) = Σ_{d|n} d * R[d] */
static size_t alloc_cap = 0;   /* current allocated size */

/* Grow the arrays to hold indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if (need + 1 <= alloc_cap)
        return;

    size_t new_cap = (alloc_cap == 0) ? 1 : alloc_cap * 2;
    while (new_cap <= (size_t)need + 1)
        new_cap *= 2;

    R      = (seq_t *)realloc(R,      new_cap * sizeof(seq_t));
    F      = (seq_t *)realloc(F,      new_cap * sizeof(seq_t));
    sigma  = (seq_t *)realloc(sigma,  new_cap * sizeof(seq_t));
    if (!R || !F || !sigma)
        abort();

    /* Initialise new cells to zero. */
    for (size_t i = alloc_cap; i < new_cap; ++i) {
        R[i]     = (seq_t)0;
        F[i]     = (seq_t)0;
        sigma[i] = (seq_t)0;
    }

    alloc_cap = new_cap;
}

/* Compute rooted-tree counts R[n] and σ[n] for 1 ≤ n ≤ N. */
static void build_rooted(idx_t N)
{
    if (N < 1)
        return;

    ensure_capacity(N);

    if (R[1] == (seq_t)0) {
        R[1]     = (seq_t)1;           /* base case */
        sigma[1] = (seq_t)1;           /* σ(1) = 1 * R(1) */
    }

    for (idx_t n = 2; n <= N; ++n) {
        /* R[n] = 1/(n-1) * Σ_{k=1}^{n-1} σ[k] * R[n-k] */
        seq_t ssum = (seq_t)0;
        for (idx_t k = 1; k < n; ++k)
            ssum += sigma[k] * R[n - k];
        R[n] = ssum / (seq_t)(n - 1);

        /* σ[n] = Σ_{d|n} d * R[d]  (now R[d] is known for all d|n) */
        seq_t sig = (seq_t)0;
        for (idx_t d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                sig += (seq_t)d * R[d];
                if (d != n / d)
                    sig += (seq_t)(n / d) * R[n / d];
            }
        }
        sigma[n] = sig;
    }
}

/* Compute free-tree counts F[n] (A000055) for 1 ≤ n ≤ N using already known R[n]. */
static void build_free(idx_t N)
{
    if (N < 1)
        return;

    ensure_capacity(N);

    if (F[1] == (seq_t)0)
        F[1] = (seq_t)1;   /* base case */

    for (idx_t n = 2; n <= N; ++n) {
        /* Σ_{i=1}^{n-1} R[i] * R[n-i] */
        seq_t sum2 = (seq_t)0;
        for (idx_t i = 1; i < n; ++i)
            sum2 += R[i] * R[n - i];

        /* If n is even subtract R[n/2] once before halving. */
        seq_t subtract = (n % 2 == 0) ? R[n / 2] : (seq_t)0;
        seq_t diff = sum2 - subtract;          /* guaranteed even */
        F[n] = R[n] - (diff / (seq_t)2);        /* free-tree count */
    }
}

/* Return a(n) = A000055(n) – 2 for n ≥ 4. */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;          /* undefined below offset */

    build_rooted(n);
    build_free(n);

    return F[n] - (seq_t)2;
}
