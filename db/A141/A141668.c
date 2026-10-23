/*
 * A141668  a(n) = tau(n) * (NumberOfPartitions(n) - 1).
 * https://oeis.org/A141668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n) * (A000041(n) - 1) = tau(n) * (p(n)-1)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 107, u64 n <= 377,
 *            u128 n <= 1367, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0u) {
        return (seq_t)0;   /* a(0) = 0 */
    }

    /* Allocate arrays for divisor counts tau[i] and partition numbers p[i] */
    seq_t *tau = alloc_seq_array(n + 1);
    seq_t *p   = alloc_seq_array(n + 1);
    if (!tau || !p) {
        seq_free_array(tau);
        seq_free_array(p);
        return (seq_t)0;   /* allocation failure */
    }

    /* ---- 1. Compute τ(i) for 1 ≤ i ≤ n ------------------------------------------------ */
    for (idx_t i = 0; i <= n; ++i) {
        tau[i] = (seq_t)0;
    }
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            tau[j] = tau[j] + (seq_t)1;
        }
    }

    /* ---- 2. Compute partition numbers p[i] with Euler’s pentagonal theorem -------- */
    p[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1;; ++k) {
            /* Generalised pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m) {
                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > m) break;
            }
            if (pent1 <= m) {
                if (k & 1u)
                    sum = sum + p[m - pent1];
                else
                    sum = sum - p[m - pent1];
            }

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= m) {
                if (k & 1u)
                    sum = sum + p[m - pent2];
                else
                    sum = sum - p[m - pent2];
            }
        }
        p[m] = sum;
    }

    /* ---- 3. Compute a(n) = τ(n) * (p(n)-1) ------------------------------------------- */
    seq_t part_minus_one = p[n] - (seq_t)1;
    seq_t result         = tau[n] * part_minus_one;

    /* Clean up */
    seq_free_array(p);
    seq_free_array(tau);

    return result;
}
