/*
 * A140697  Mobius transform of A000082.
 * https://oeis.org/A140697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(p) = p*(p+1)-1 = p^2+p-1 a(p^e) =
 *            (p-1)*(p+1)^2*p^(2e-3) for e ≥ 2 (OEIS A140697)
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>                 /* size_t */

#include "../utils/factor_u64.h"    /* factor_u64 */
#include "../utils/pow_int.h"       /* pow_int(seq_t, idx_t) */

static seq_t prime_power_term(seq_t p, uint64_t e)
{
    if (e == 1) {
        /* p^2 + p - 1 */
        return p * (p + (seq_t)1) - (seq_t)1;
    } else {
        /* (p-1)*(p+1)^2 * p^(2e-3) */
        seq_t base = (p - (seq_t)1) *
                     ((p + (seq_t)1) * (p + (seq_t)1));
        idx_t exp = (idx_t)((2 * e) - 3);
        seq_t pow = pow_int(p, exp);
        return base * pow;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1 */
        return (seq_t)1;

    /* Factor n using the provided routine.  For any 64‑bit n,
       at most 64 distinct primes occur. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        result *= prime_power_term(p, e);
    }
    return result;
}
