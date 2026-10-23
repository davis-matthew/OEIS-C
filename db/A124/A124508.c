/*
 * A124508  a(n) = 2^BigO(n) * 3^omega(n), where BigO = A001222 and omega =
 *          A001221, the numbers of prime factors of n with and without
 *          repetitions.
 * https://oeis.org/A124508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^BigO(n) * 3^omega(n)
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    /* Factor n into distinct primes and their exponents.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute sum of exponents (BigO) and number of distinct primes (omega). */
    idx_t bigO = 0;
    for (size_t i = 0; i < cnt; ++i)
        bigO += (idx_t)exps[i];
    idx_t omega = (idx_t)cnt;

    /* a(n) = 2^BigO * 3^omega.  */
    return pow2(bigO) * pow3(omega);
}
