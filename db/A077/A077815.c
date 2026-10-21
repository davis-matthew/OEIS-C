/*
 * A077815  2^phi(n) mod n^2, where phi is Euler's totient function A000010.
 * https://oeis.org/A077815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^phi(n) mod n^2
 * Range:     u8 n <= 15, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for OEIS, but safe fallback */

    seq_t mod = (seq_t)n * ((seq_t)n);
    seq_t phi_n = phi(n);          // phi(n) fits in uint64_t for n ≤ 10⁴
    seq_t result = mod_pow((seq_t)2, (uint64_t)phi_n, mod);
    return result;
}
