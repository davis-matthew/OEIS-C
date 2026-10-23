/*
 * A190116  a(n) = n*n', where n' is the arithmetic derivative (A003415) of n.
 * https://oeis.org/A190116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{p^e‖n} e * n² / p 
 * (where the sum runs over all prime powers dividing n, 
 *  p is the prime and e its exponent in the factorisation)
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* safety – offset is 1, but guard a(0) */
        return 0;

    uint64_t nn = (uint64_t)n;           /* use native 64‑bit for factorisation   */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt   = 0;

    /* factorise n into primes/primes^exponents */
    factor_u64(nn, primes, exps, &cnt);

    /* compute a(n) = Σ e * n² / p  */
    seq_t result = 0;
    seq_t n_seq  = (seq_t)nn;
    seq_t n2     = n_seq * n_seq;        /* n² fits in 128‑bit when nn≤2⁶⁴−1 */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = n2 / (seq_t)primes[i];
        term *= (seq_t)exps[i];
        result += term;
    }

    return result;
}
