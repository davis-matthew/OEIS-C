/*
 * A089692  a(n) = phi(2n)/2^omega(n).
 * https://oeis.org/A089692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = phi(2n)/2^omega(n) (OEIS A089692)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the multiplicative definition:
   a(2^e)      = 2^(e-1)
   a(p^e)      = (p^e - p^(e-1))/2      for odd prime p
   For a general n = Π p_i^e_i,  a(n) = Π a(p_i^e_i). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/pow2.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 1)            /* idx_t is unsigned, only true when n == 0 */
        return 0;

    if (n == 1)
        return (seq_t)1;  /* a(1) = 1 */

    /* Factorise n.  64 primes are enough for 64‑bit inputs. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t   term;

        if (p == 2) {
            /* a(2^e) = 2^(e-1) */
            term = pow2((idx_t)(e - 1));
        } else {
            /* a(p^e) = (p^e - p^(e-1))/2  (p odd prime) */
            seq_t pe  = pow_seq((seq_t)p, (int)e);
            seq_t pe1 = pow_seq((seq_t)p, (int)(e - 1));
            term = (pe - pe1) / (seq_t)2;
        }
        result = result * term;
    }
    return result;
}
