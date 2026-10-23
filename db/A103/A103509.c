/*
 * A103509  a(n) is the least j such that 2n+1 = 2*A000040(k) + A000040(j) for
 *          some k > 1, or 0 if no such j exists.
 * https://oeis.org/A103509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least j such that 2*n+1 = 2*A000040(k) + A000040(j) for
 *            some k > 1, or 0 if no such j exists
 * Range:     u8 n <= 8830, u16 n <= 8830, u32 n <= 8830, u64 n <= 8830,
 *            u128 n <= 8830, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; a(1), a(2), a(3) are 0 by definition. */
    if (n < 4) {
        return 0;
    }

    /* target = 2*n + 1  as a 64‑bit integer (fits for all n required). */
    uint64_t target = (uint64_t)(2 * n) + 1;

    /* The largest prime pj we can use is when k=2 (prime(k)=3). */
    uint64_t max_pj = (target > 3 ? target - 3 : 0);

    for (uint64_t j = 1; ; ++j) {
        /* pj = A000040(j). */
        seq_t pj_seq = get_prime(j);
        uint64_t pj = (uint64_t)pj_seq;

        if (pj > max_pj) {
            break;               /* No further j can satisfy the equation. */
        }

        uint64_t d = target - pj;    /* d = 2*prime(k).   */
        if ((d & 1ULL) != 0) {       /* d must be even.   */
            continue;
        }

        uint64_t q = d >> 1;        /* Candidate prime(k). */
        if (q >= 3 && is_prime(q)) {
            return (seq_t)j;        /* Smallest such j found. */
        }
    }

    return 0;                       /* No suitable j exists.   */
}
