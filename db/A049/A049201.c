/*
 * A049201  If n is not composite, a(n) = n followed by 1; if n is composite,
 *          a(n) = concatenation of prime factors of n.
 * https://oeis.org/A049201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   if n is composite, a(n) = concatenation of prime factors of n;
 *            otherwise a(n) = concatenation of n and 1 (i.e., n followed by
 *            digit 1)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digits, factor_u64, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/factor_u64.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle the special cases n = 0 and n = 1 */
    if (n == 0)
        return 1;
    if (n == 1)
        return 11;

    /* If n is prime (or 0/1 handled above) append a trailing digit 1 */
    if (is_prime_small((seq_t)n))
        return ((seq_t)n) * 10 + 1;

    /* Composite case: factor n and concatenate prime factors in ascending
       order (with multiplicity).  The factor_u64 routine returns each prime
       factor once with its exponent.  We expand these exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    bool first = true;           /* first factor is used as the seed */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        for (int j = 0; j < (int)exps[i]; ++j) {
            if (first) {
                result = (seq_t)p;
                first = false;
            } else {
                int d = digits((idx_t)p);          /* number of decimal digits of p */
                seq_t mult = pow10((idx_t)d);       /* 10^d as seq_t */
                result = result * mult + (seq_t)p;
            }
        }
    }

    return result;
}
