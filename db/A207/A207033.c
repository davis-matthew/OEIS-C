/*
 * A207033  Total number of parts >= 3 in all partitions of n.
 * https://oeis.org/A207033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{r=0}^{n-3} p(r) * ( τ(n‑r) – 1 – ( (n‑r) mod 2 ) )
 *            where p(k) is the number of partitions of k and τ(m) is the
 *            number of positive divisors of m. This follows from c_k(n) =
 *            ∑_{t≥1} p(n‑t·k) for k≥3, hence a(n)=∑_{k≥3}c_k(n)
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 106, u64 n <= 362,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) {
        return (seq_t)0;
    }

    /* static state for memoisation */
    static idx_t cur_max = 0;
    static seq_t *p_arr = NULL;    /* partition numbers */
    static size_t *tau_arr = NULL; /* divisor counts */

    /* extend the pre‑computed tables if necessary */
    if (n > cur_max) {
        idx_t new_max = n;

        seq_t *new_p = (seq_t*)realloc(p_arr, (new_max + 1) * sizeof(seq_t));
        size_t *new_tau = (size_t*)realloc(tau_arr, (new_max + 1) * sizeof(size_t));
        if (!new_p || !new_tau) {
            /* allocation failed – return 0 (rare in the test harness) */
            return (seq_t)0;
        }
        p_arr = new_p;
        tau_arr = new_tau;

        /* initialise partition array */
        p_arr[0] = (seq_t)1;
        for (idx_t i = 1; i <= new_max; ++i)
            p_arr[i] = (seq_t)0;

        /* compute partition numbers p(i) for i ≤ new_max */
        for (idx_t coin = 1; coin <= new_max; ++coin) {
            for (idx_t sum = coin; sum <= new_max; ++sum) {
                p_arr[sum] += p_arr[sum - coin];
            }
        }

        /* initialise divisor‑count array */
        for (idx_t i = 0; i <= new_max; ++i)
            tau_arr[i] = 0;

        /* sieve to compute τ(i) */
        for (idx_t div = 1; div <= new_max; ++div) {
            for (idx_t mult = div; mult <= new_max; mult += div) {
                ++tau_arr[mult];
            }
        }

        cur_max = new_max;
    }

    /* compute the desired sum */
    seq_t result = (seq_t)0;
    for (idx_t r = 0; r <= n - 3; ++r) {
        idx_t m = n - r;                       /* m = n - r ≥ 3 */
        size_t tau_ge3 = tau_arr[m] - 1;       /* remove divisor 1 */
        if ((m & 1) == 0) {                   /* subtract divisor 2 if present */
            --tau_ge3;
        }
        seq_t prod = p_arr[r] * (seq_t)tau_ge3;/* p(r) * count(divisors ≥3 of m) */
        result += prod;
    }
    return result;
}
