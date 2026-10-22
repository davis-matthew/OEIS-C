/*
 * A002267  The 15 supersingular primes: primes dividing order of Monster
 *          simple group.
 * https://oeis.org/A002267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   search, not a closed form. Primes p = 2, 3 are always terms. A
 *            prime p >= 5 is a term (supersingular) iff every supersingular
 *            j-invariant in characteristic p lies in F_p. The total count is
 *            floor(p/12) + {0,1,1,2} for p == 1,5,7,11 (mod 12). The count
 *            lying in F_p is h(-4p)/2 for p == 1 (mod 4),...
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   a2267_gcd, a2267_h, a2267_isprime, a2267_supersingular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2267_gcd.h"
#include "../utils/a2267_h.h"
#include "../utils/a2267_isprime.h"
#include "../utils/a2267_supersingular.h"

seq_t a(idx_t n)
{
    idx_t k = 0;
    long long p;
    for (p = 2; p <= 2000; p++) {
        if (!a2267_isprime(p)) continue;
        if (a2267_supersingular(p)) {
            k++;
            if (k == n) return (seq_t)p;
        }
    }
    return (seq_t)0;
}
