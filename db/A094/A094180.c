/*
 * A094180  Numbers k such that 4*k-1 is divisible only by primes of form
 *          4*m-1 (i.e., by the Gaussian primes A002145).
 * https://oeis.org/A094180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that every prime divisor of
 *            (4*k-1) is ≡3 (mod 4)
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is directly the definition of A094180. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t k = 1;      /* candidate k */
    idx_t count = 0;  /* number of valid k found so far */

    for ( ;; ++k ) {
        uint64_t N = 4ULL * k - 1ULL;   /* 4*k-1, fits in 64 bits for required n */
        uint64_t rem = N;
        bool ok = true;

        /* trial division by odd numbers starting at 3 */
        for (uint64_t d = 3ULL; d * d <= rem; d += 2ULL) {
            if (rem % d == 0ULL) {
                /* d is a prime factor */
                if ((d & 3ULL) == 1ULL) {   /* d ≡ 1 (mod 4) → reject */
                    ok = false;
                    break;
                }
                while (rem % d == 0ULL) {
                    rem /= d;
                }
            }
        }
        /* after loop, rem may be 1 or a remaining prime factor */
        if (ok && rem > 1ULL) {
            if ((rem & 3ULL) == 1ULL) {   /* remaining prime ≡ 1 (mod 4) → reject */
                ok = false;
            }
        }

        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
