/*
 * A081124  Binomial transform of floor(n/2)!.
 * https://oeis.org/A081124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n,k) * floor(k/2)!
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 36, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an O(n) iterative algorithm. */

#if defined(__cplusplus)
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* Pre‑compute factorials up to floor(n/2) */
    idx_t max_fact = n >> 1;  /* floor(n/2) */
#if defined(__cplusplus)
    std::vector<seq_t> fact(max_fact + 1);
#else
    /* In the C build seq_t is an unsigned integer type, so simple malloc is enough */
    seq_t *fact = (seq_t *)malloc((max_fact + 1) * sizeof(seq_t));
    if (!fact)
        return 0;   /* Out of memory – return 0 as a safe default */
#endif

    /* fact[0] = 1 */
#if defined(__cplusplus)
    fact[0] = 1;
#else
    fact[0] = 1;
#endif
    for (idx_t i = 1; i <= max_fact; ++i)
    {
#if defined(__cplusplus)
        fact[i] = fact[i - 1] * (seq_t)i;
#else
        fact[i] = fact[i - 1] * (seq_t)i;
#endif
    }

    seq_t binom = 1;   /* binomial(n,0) */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Add term binom * fact[floor(k/2)] */
        sum += binom * fact[k >> 1];

        if (k == n)
            break;   /* binomial update not needed for last iteration */

        /* Update binomial for next k: binom = binom * (n-k) / (k+1) */
        binom = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);
    }

#if !defined(__cplusplus)
    free(fact);
#endif

    return sum;
}
