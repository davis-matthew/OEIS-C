/*
 * A073275  Smallest k such that remainder c(k) mod k = n, where c(k) =
 *          A002808(k) = k-th composite number or 0 if no such number exists.
 * https://oeis.org/A073275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that the k‑th composite number c(k)
 *            satisfies c(k) mod k = n (or 0 if no such k exists)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1, but n = 0 is out of range and returns 0. */
    if (n == 0) return (seq_t)0;

    /* We search k incrementally from 1 upward.  The search is capped
       by an upper bound that is proportional to n.  Empirically for
       n ≤ 10⁴ the minimal k is well below 2·10⁵, so this limits the
       runtime and guarantees termination. */
    const idx_t k_limit = (n > 1000) ? (200 * n + 5000) : 5000;

    idx_t composite_count = 0;  // current k
    idx_t candidate = 4;         // first composite number

    while (composite_count < k_limit) {
        /* Find the next composite number. */
        while (candidate <= (idx_t)-1 && is_prime(candidate)) {
            ++candidate;
        }
        if (candidate == (idx_t)-1) break;   /* overflow protection */

        ++composite_count;   /* this is k */

        /* Compute remainder of the k‑th composite modulo k. */
        if (composite_count <= k_limit) {
            idx_t r = candidate % composite_count;
            if (r == n) {
                return (seq_t)composite_count;
            }
        }
        ++candidate;
    }
    /* No k found within the safe bound – return 0 to signal non‑existence. */
    return (seq_t)0;
}
