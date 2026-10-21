/*
 * A095972  Number of quadratic nonresidues modulo n.
 * https://oeis.org/A095972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A000224(n) Where A000224(n) is the number of
 *            quadratic residues modulo n
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   residues_power_of_two, residues_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A000224(n) through prime factorisation.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/residues_power_of_two.h"
#include "../utils/residues_prime_power.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;               /* a(1) = 0 */

    uint64_t rem = (uint64_t)n;
    seq_t residues = 1;                        /* multiplicative identity */

    /* handle factor 2 */
    if ((rem & 1ULL) == 0) {
        uint32_t e = 0;
        while ((rem & 1ULL) == 0) {
            rem >>= 1;
            ++e;
        }
        residues *= residues_power_of_two(e);
    }

    /* factor odd primes by trial division */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            uint32_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            residues *= residues_prime_power(p, e);
        }
    }

    /* remaining prime factor > sqrt(rem) */
    if (rem > 1) {
        /* rem is prime, exponent e = 1 */
        residues *= residues_prime_power(rem, 1);
    }

    return (seq_t)n - residues;
}
