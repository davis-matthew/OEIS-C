/*
 * A069091  Jordan function J_6(n).
 * https://oeis.org/A069091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^(6e) - p^(6(e-1))) (Jordan function J₆)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 2000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(1)=1, a(2)=63, a(3)=728, ... as defined for OEIS A069091 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for the intended sequence; the behaviour for n = 0 is irrelevant. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factor n into its prime powers.  The helper writes into primes[] and exps[]
       and sets *cnt to the number of distinct primes. */
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* Compute p^(6e) and p^(6(e-1)).  For e=1, the second term is p^0 = 1. */
        seq_t pow_high = big_pow(p, (seq_t)(6 * e));
        seq_t pow_low  = big_pow(p, (seq_t)(6 * (e - 1)));

        result = result * (pow_high - pow_low);
    }

    return result;
}
