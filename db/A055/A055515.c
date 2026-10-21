/*
 * A055515  a(n) = (2^n - 1)/product(2^p - 1) where the product is over all
 *          distinct primes p that divide n.
 * https://oeis.org/A055515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - 1) / ∏_{p|n} (2^p - 1) where the product is over
 *            all distinct primes p dividing n
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 71, u128 n <= 142,
 *            big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety for non‑offset calls */

    /* numerator (2^n - 1) */
    seq_t num = pow2(n) - (seq_t)1;

    /* denominator product over distinct prime factors of n */
    seq_t den = (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t tmp = pow2(primes[i]) - (seq_t)1;
        den = den * tmp;   /* exact product in seq_t */
    }

    return num / den;   /* denominator divides numerator exactly */
}
