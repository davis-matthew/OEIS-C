/*
 * A174621  a(n) is the smallest generator for the group of numbers relatively
 *          prime to n under multiplication mod n; a(n) = -1 if n is not a
 *          power of a prime or twice a power of a prime.
 * https://oeis.org/A174621
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   Computes the smallest primitive root modulo n (multiplicative
 *            group of units mod n). The value returned is the smallest g such
 *            that g^(φ(n)/q) ≠ 1 (mod n) for every prime divisor q of φ(n).
 *            If n is not a prime power or twice a prime power, the group is
 *            not cyclic and the function returns –1. (The...
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   factor_u64, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 2)
        return (seq_t)1;

    /* Determine whether n lies in the class of cyclic groups.  The
       multiplicative group (Z/nZ)* is cyclic iff
         n = 2,
         n = 4,
         n = p^k for an odd prime p,
         n = 2·p^k for an odd prime p. */
    if (n == 4) {
        /* n is allowed — fall through to compute the primitive root. */
    } else if (n % 2 == 0) {
        /* Even n larger than 2 */
        if (n % 4 == 0)              /* e.g. 8, 12, … */
            return (seq_t)-1;         /* not of the required form */
        idx_t m = n / 2;              /* n = 2·m, m must be an odd prime power */
        uint64_t primes[64], exps[64];
        size_t cnt = 0;
        factor_u64(m, primes, exps, &cnt);
        if (cnt != 1 || primes[0] == 2)
            return (seq_t)-1;         /* m is not an odd prime power */
    } else {
        /* Odd n must be a prime power */
        uint64_t primes[64], exps[64];
        size_t cnt = 0;
        factor_u64(n, primes, exps, &cnt);
        if (cnt != 1)
            return (seq_t)-1;         /* not a prime power */
    }

    /* Compute φ(n) */
    uint64_t primes_n[64], exps_n[64];
    size_t cnt_n = 0;
    factor_u64(n, primes_n, exps_n, &cnt_n);
    uint64_t phi_u = (uint64_t)n;
    for (size_t i = 0; i < cnt_n; ++i) {
        uint64_t p = primes_n[i];
        phi_u = (uint64_t)((__int128)phi_u / p * (p - 1));
    }

    /* Factor φ(n) to obtain its distinct prime divisors. */
    uint64_t primes_phi[64], exps_phi[64];
    size_t cnt_phi = 0;
    factor_u64(phi_u, primes_phi, exps_phi, &cnt_phi);

    /* Search for the smallest primitive root. */
    for (idx_t g = 2; g < n; ++g) {
        if (gcd((seq_t)g, (seq_t)n) != 1)
            continue;
        bool ok = true;
        for (size_t j = 0; j < cnt_phi; ++j) {
            uint64_t q = primes_phi[j];
            seq_t r = mod_pow((seq_t)g, phi_u / q, (seq_t)n);
            if (r == 1) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)g;
    }

    /* In theory we should never reach this point for a cyclic n. */
    return (seq_t)-1;
}
