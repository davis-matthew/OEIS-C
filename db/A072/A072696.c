/*
 * A072696  a(n) = lcm(d(n^3), d(n)), where d(n) = A000005, the number of
 *          divisors of n.
 * https://oeis.org/A072696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(d(n^3), d(n))
 * Range:     u8 n <= 23, u16 n <= 9914, u32 n <= 9914, u64 n <= 9914,
 *            u128 n <= 9914, big any n
 * Helpers:   factor_u64, seq_gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_gcd.h"

/* Helper: compute divisor counts of n and n^3 */
static void divisor_counts_u64(uint64_t n, seq_t *d, seq_t *d3)
{
    /* 64‑bit integers have at most a few prime factors, but allocate a bit
       more to be safe. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    seq_t prod_d  = 1;
    seq_t prod_d3 = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t e  = (seq_t)exps[i];
        seq_t val_d  = e + 1;      /* (e + 1)            */
        seq_t val_d3 = 3 * e + 1;  /* (3e + 1)           */
        prod_d  = seq_mul(prod_d,  val_d);
        prod_d3 = seq_mul(prod_d3, val_d3);
    }

    *d  = prod_d;
    *d3 = prod_d3;
}

seq_t a(idx_t n)
{
    seq_t d, d3;
    divisor_counts_u64((uint64_t)n, &d, &d3);

    /* lcm(d, d3) = d * d3 / gcd(d, d3) */
    seq_t g   = seq_gcd(d, d3);
    seq_t prod = seq_mul(d, d3);
    return prod / g;
}
