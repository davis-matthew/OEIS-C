/*
 * A046530  Number of distinct cubic residues mod n.
 * https://oeis.org/A046530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, nice
 * Formula:   a(n) = number of distinct cubic residues mod n. * The sequence
 *            is multiplicative. For a prime power p^e (e ≥ 1) * a(p^e) = 1 +
 *            Σ_{t=0..⌊(e-1)/3⌋} φ(p^{e-3t}) / gcd(3, φ(p^{e-3t})) * where
 *            φ(p^k) = p^{k-1}(p-1). For e = 0 (n=1) the value is 1
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, prime_power_cubic_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function assumes that the type seq_t supports the usual
 * integer arithmetic operators.  In a C++ build seq_t may be an
 * arbitrary‐precision class; in a C build it is an unsigned integer type. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_power_cubic_count.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined but safe */
    if (n == 1) return 1;        /* a(1) = 1 */

    uint64_t primes[32];
    uint64_t exps[32];
    size_t    cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t part = prime_power_cubic_count(primes[i], exps[i]);
        result *= part;
    }
    return result;
}
