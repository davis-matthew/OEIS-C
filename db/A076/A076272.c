/*
 * A076272  Largest prime factor of A076271(n): A006530(A076271(n)).
 * https://oeis.org/A076272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, let S_1 = 1 and S_k = S_{k-1} + a(k-1).
 *            Then a(k) = largest_prime_factor(S_k). This implements the
 *            recurrence a(n) = A076271(n+1) – A076271(n) where A076271 is
 *            defined by : A076271(1)=1, A076271(k+1)=A076271(k)+a(k). No
 *            caching or dynamic allocation is required
 * Range:     u8 n <= 31, u16 n <= 965, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* safety for off‑by‑one: the OEIS offset is 1, so n≥1 */
    if (n == 0) return 0;

    /* a(1) = 1, S_1 = 1 */
    seq_t sum      = 1;   /* current S_{k-1} during the loop */
    seq_t prev_a   = 1;   /* a(k-1) */

    if (n == 1) return 1;

    /* iterate from k = 2 up to n */
    for (idx_t k = 2; k <= n; ++k) {
        /* S_k = S_{k-1} + a(k-1) */
        seq_t new_sum = sum + prev_a;

        /* largest prime factor of S_k, computed in uint64_t domain */
        uint64_t sum64 = (uint64_t)new_sum;
        uint64_t lpf64 = largest_prime_factor(sum64);
        seq_t a_k = lpf64;   /* implicit conversion to seq_t */

        if (k == n) return a_k;   /* found requested term */

        /* prepare for next iteration */
        sum     = new_sum;
        prev_a  = a_k;
    }

    /* unreachable for valid n */
    return 0;
}
