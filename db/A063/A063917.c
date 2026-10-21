/*
 * A063917  Smallest k such that k!!! is a multiple of n.
 * https://oeis.org/A063917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   exp_in_triple_fact, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula:
 *  a(n) = smallest k such that k!!! (triple factorial) is a multiple of n
 *  i.e., k * (k-3) * (k-6) * … ≥ 1 and divisible by n.
 *  This implementation factors n and tests successive k until the
 *  required multiplicity of every prime factor of n is satisfied.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* for factor_u64 */
#include "../utils/exp_in_triple_fact.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* trivial case */

    uint64_t nn = (uint64_t)n;
    /* Factor n: maximum 64 primes for 64‑bit values */
    uint64_t primes[64];
    uint64_t exps [64];
    size_t   num = 0;
    factor_u64(nn, primes, exps, &num);

    /* Search k from 1 upward (k ≤ n always works) */
    for (uint64_t k = 1; k <= nn; ++k) {
        bool ok = true;
        for (size_t i = 0; i < num; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];
            if (exp_in_triple_fact(k, p, e) < e) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)k;   /* minimal k found */
    }

    /* Should never reach here: fallback to n */
    return (seq_t)nn;
}
