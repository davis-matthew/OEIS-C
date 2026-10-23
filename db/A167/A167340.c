/*
 * A167340  Totally multiplicative sequence with a(p) = p*(p+2) = p^2+2p for
 *          prime p.
 * https://oeis.org/A167340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p*(p+2))^e (totally multiplicative,
 *            a(p)=p*(p+2))
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* helper prototypes provided by the build */
#include "../utils/factor_u64.h"  /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t base, idx_t exp); */

seq_t a(idx_t n)
{
    if (n == 0)              /* offset starts at 1; 0 is out‑of‑range */
        return 0;

    if (n == 1)              /* a(1) = 1 by definition */
        return 1;

    static uint64_t primes[64];
    static uint64_t exps[ 64];
    size_t cnt = 0;

    /* factorize n into prime powers */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    /* multiply contributions of each prime factor */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        seq_t  base  = (seq_t)p * ((seq_t)p + 2);
        seq_t  term  = seq_pow(base, (idx_t)e);   /* (p*(p+2))^e */
        result *= term;
    }

    return result;
}
