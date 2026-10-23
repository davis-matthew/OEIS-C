/*
 * A123926  Greatest common divisor of sigma_k(n) for all k >= 1.
 * https://oeis.org/A123926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = gcd_{k ≥ 1} σ_k(n) for A123926.
   For each n we factor it, compute τ(n) = σ_0(n),
   and test all divisors d of τ(n), in decreasing order.
   A divisor d divides all σ_k(n) iff for all k = 1 … τ(n),
   σ_k(n) ≡ 0 (mod d).  The largest such d is the answer. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/mod_pow.h"

/* Helper: test whether a divisor d divides σ_k(n) for all k = 1 … τ(n). */
static bool divisor_divides_all_sigma(uint64_t n, uint64_t d, uint64_t tau)
{
    /* 0 divisor is impossible; 1 always works but is handled outside. */
    if (d == 1) return true;

    /* Factor n.  <16 distinct prime factors for n ≤ 10 000. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t md = (seq_t)d;                 /* modulus as seq_t */

    for (uint64_t k = 1; k <= tau; ++k) {
        seq_t sigma = 1 % md;            /* product of sums for each prime */

        for (size_t pi = 0; pi < cnt; ++pi) {
            /* pk = p^k (mod d) */
            seq_t pk_mod = mod_pow((seq_t)(primes[pi] % d), k, md);

            /* Sum_{i=0}^{e} (pk_mod)^i  (mod d) */
            seq_t sum   = 1 % md;        /* term for i = 0 */
            seq_t term  = 1 % md;        /* current power (pk_mod)^i */
            for (uint64_t i = 1; i <= exps[pi]; ++i) {
                term = (term * pk_mod) % md;
                sum += term;
                if (sum >= md) sum -= md;
            }

            sigma = (sigma * sum) % md;
            if (sigma == 0) break;      /* product already zero modulo d */
        }

        if (sigma != 0) return false;   /* d does not divide all σ_k(n) */
    }
    return true;                         /* d divides all σ_k(n) */
}

seq_t a(idx_t n)
{
    /* n ≥ 1 as per sequence offset. */
    uint64_t N = (uint64_t)n;

    /* τ(n) = σ_0(n) = number of divisors of n. */
    uint64_t tau = divisor_count_u64(N);

    /* Test divisors of τ(n) in decreasing order. */
    for (uint64_t d = tau; d >= 1; --d) {
        if (tau % d != 0) continue;     /* d must divide τ(n) */

        /* d = 1 always works; otherwise check explicitly. */
        if (d == 1 || divisor_divides_all_sigma(N, d, tau))
            return (seq_t)d;
    }
    return (seq_t)1;   /* should never reach here */
}
