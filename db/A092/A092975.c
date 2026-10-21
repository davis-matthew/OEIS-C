/*
 * A092975  Consider all partitions of n into parts all of which are divisors
 *          of n; a(n) = maximal product of parts.
 * https://oeis.org/A092975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ (n/d)^d : d divides n } . By the theory in OEIS
 *            A092975, a(n) = 3^(n/3) if 3 | n, otherwise a(n) = q^(n/q) where
 *            q is the smallest prime factor of n
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 62, u64 n <= 122,
 *            u128 n <= 242, big any n
 * Helpers:   _smallest_prime_factor, ipow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"
#include "../utils/ipow_seq_t.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1. */
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    /* Find smallest prime factor q of n.  The helper works on uint64_t;
       n is guaranteed to fit into that type (n >= 1). */
    uint64_t q64 = _smallest_prime_factor((uint64_t)n);
    idx_t q = (idx_t)q64;          /* Convert to idx_t for exponent argument. */

    /* If 3 divides n, use the special formula. */
    if ((n % (idx_t)3) == (idx_t)0) {
        idx_t exp = n / (idx_t)3;
        return ipow_seq_t((seq_t)3, exp);
    }

    /* Otherwise use q^(n/q).  Here n/q >= 1 because q <= n. */
    idx_t exp = n / q;
    return ipow_seq_t((seq_t)q, exp);
}
