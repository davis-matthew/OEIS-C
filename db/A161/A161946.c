/*
 * A161946  Odd part of sum of unitary divisors of n.
 * https://oeis.org/A161946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = oddpart(∏_{p^e‖n} (p^e + 1))
 * Range:     u8 n <= 254, u16 n <= 9503, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   factor_u64, odd_part, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/odd_part.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          // sequence is defined for n >= 1
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t term = seq_pow(p, exps[i]);   /* p^e */
        term += 1;                          /* (p^e + 1) */
        prod *= term;
    }
    return odd_part(prod);
}
