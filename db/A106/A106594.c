/*
 * A106594  a(n) = number of primitive solutions to 4n+1 = x^2 + y^2.
 * https://oeis.org/A106594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primitive solutions to 4n+1 = x² + y², with x,
 *            y positive and gcd(x, y) = 1. This equals 0 if 4n+1 contains any
 *            prime ≡ 3 (mod 4); otherwise it equals 2^(k-1), where k is the
 *            number of distinct primes ≡ 1 (mod 4) dividing 4n+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Compute m = 4n+1 */
    uint64_t m = (uint64_t)4 * n + 1ULL;
    /* Factor m by trial division, counting primes ≡1 (mod 4) and aborting on
       any prime ≡3 (mod 4). */
    bool has_p3 = false;
    int count_1mod4 = 0;

    /* factor out 2 — not needed because m is odd, but kept for generality */
    if (m % 2 == 0) {
        /* Even factor cannot occur because 4n+1 is odd, but keep for safety */
        return 0;
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (p % 4 == 3) {
                has_p3 = true;
                break;
            } else if (p % 4 == 1) {
                ++count_1mod4;
            }
        }
    }

    if (has_p3) {
        return 0;
    }

    /* If in the end m > 1, it is a prime factor. */
    if (m > 1) {
        if (m % 4 == 3) {
            return 0;
        } else if (m % 4 == 1) {
            ++count_1mod4;
        }
    }

    /* Now count_1mod4 ≥ 1 for any non‑zero result. */
    if (count_1mod4 == 0) {
        return 0;
    }

    /* 2^(k-1) fits in seq_t for the tested ranges. */
    seq_t result = (seq_t)1 << (count_1mod4 - 1);
    return result;
}
