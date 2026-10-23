/*
 * A103946  Indices of icosahedral numbers (A006564) which are semiprimes.
 * https://oeis.org/A103946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103946 – indices k such that the icosahedral number A006564(k) is a semiprime.
 *
 * The icosahedral number is M(k) = k * (5*k^2 - 5*k + 2) / 2.
 * The OEIS definition is Bigomega(M(k)) = 2, i.e. M(k) has exactly two prime
 * factors counted with multiplicity.  For any k, this is equivalent to
 * k being prime and (5*k^2 - 5*k + 2)/2 being prime, because if k were
 * composite M(k) would have at least three prime factors.
 *
 * Algorithm: iterate over integers k starting at 2, test k for primality,
 * then compute q = (5*k^2 - 5*k + 2)/2 and test q for primality.
 * When both are prime the k is recorded; the n‑th such k is returned.
 *
 * Complexity: roughly O(n log n) primality tests; well within limits for
 * n ≈ 10 000.  No memoisation is required, as calls may arrive in any order.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(uint64_t) – deterministic 64‑bit MR */

seq_t a(idx_t n)
{
    /* n ≥ 1 by specification. */
    idx_t count = 0;
    uint64_t k = 2;  /* start checking from the first integer > 1 */

    while (true) {
        /* Quick discard: if k is not prime, skip. */
        if (is_prime(k)) {
            /* Compute q = (5*k^2 - 5*k + 2) / 2 using 128‑bit to avoid overflow. */
            unsigned __int128 kk = (unsigned __int128)k;
            unsigned __int128 num = 5*kk*kk - 5*kk + 2;  /* numerator */
            unsigned __int128 q128 = num / 2;

            /* q must fit in 64‑bit for the available primality test. */
            if (q128 <= (unsigned __int128)UINT64_MAX) {
                uint64_t q = (uint64_t)q128;
                if (is_prime(q)) {
                    /* k is a valid term of the sequence. */
                    ++count;
                    if (count == n)
                        return (seq_t)k;
                }
            }
        }
        /* Move to the next integer.  k will overflow only after an astronomical
         * number of iterations, which will not happen for n ≈ 10 000. */
        ++k;
    }

    /* In practice this point is never reached.  Return 0 as a safe fallback. */
    return (seq_t)0;
}
