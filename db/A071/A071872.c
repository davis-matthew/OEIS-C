/*
 * A071872  Number of positive k such that k*tau(k) < n plus the number of
 *          positive k such that k*tau(k) = n-1.
 * https://oeis.org/A071872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A071872:
 *  a(n) = #{k ≥ 1 | k * τ(k) < n}  +  #{k ≥ 1 | k * τ(k) = n − 1}
 *  (τ(k) = number of positive divisors of k)
 *
 *  The function keeps a static table of divisor counts up to the
 *  largest n ever requested and updates it lazily.  Each k that
 *  satisfies k*τ(k)=n−1 is counted *twice*: once in the < n part
 *  and once explicitly for the equality part, exactly as the
 *  mathematical definition prescribes.
 */

#include <stdlib.h>

/* static storage for divisor counts */
static uint32_t *tau = NULL;   /* tau[k] = τ(k) for 0 ≤ k < size */
static idx_t tau_size = 0;    /* length of allocated array */

/* build divisor counts for all k from 1 to max_k inclusive */
static void recompute_tau(idx_t max_k)
{
    uint32_t *new_tau = (uint32_t *)malloc((max_k + 1) * sizeof(uint32_t));
    if (!new_tau) return;                 /* leave old table if out of memory */

    for (idx_t i = 0; i <= max_k; ++i) new_tau[i] = 0;

    for (idx_t i = 1; i <= max_k; ++i)
        for (idx_t j = i; j <= max_k; j += i)
            ++new_tau[j];

    free(tau);
    tau   = new_tau;
    tau_size = max_k + 1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                 /* a(1) = 0 */

    /* ensure τ(k) is known for all k < n (so up to n-1) */
    idx_t required = n - 1;
    if (required + 1 > tau_size)
        recompute_tau(required);

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t prod = (seq_t)k * (seq_t)tau[k];
        if (prod < n) ++count;          /* first condition */
        if (prod == n - 1) ++count;     /* second condition (may double count) */
    }
    return count;
}
