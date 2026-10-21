/*
 * A069817  Smallest remainder of p+q modulo n, where p*q = n^2 - 1, 1 < p <
 *          n-1, or n if no such factorization exists.
 * https://oeis.org/A069817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = smallest non‑negative remainder of (p+q) mod n among all
 *            factorizations p*q = n^2-1 with 1 < p < n-1; if no such
 *            factorization exists or if both n-1 and n+1 are prime, the value
 *            is n
 * Range:     u8 n <= 26, u16 n <= 2387, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Special case: n-1 and n+1 both prime => return n. */
    if (n > 2 && is_prime(n - 1) && is_prime(n + 1))
        return (seq_t)n;

    /* Compute n^2 - 1.  For the intended range of n (<= 10^4) this
       fits comfortably in 64 bits. */
    uint64_t target = (uint64_t)n * (uint64_t)n;
    target -= 1ULL;

    seq_t min_rem = (seq_t)n;          /* initialise with "no factor" value */

    /* Only p in (1, n-1) are considered; q is the complementary factor. */
    for (idx_t p = 2; p < n - 1; ++p) {
        if (target % p != 0)
            continue;

        uint64_t q = target / p;
        seq_t rem = ((seq_t)p + (seq_t)q) % (seq_t)n;
        if (rem < min_rem) {
            min_rem = rem;
            if (min_rem == 0)          /* cannot get smaller */
                break;
        }
    }

    return min_rem;
}
