/*
 * A034677  Sum of cubes of unitary divisors of n.
 * https://oeis.org/A034677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, changed
 * Formula:   a(n) = ∏_{p^k ‖ n} (1 + p^(3·k))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (prime powers “‖” denote the exact exponent k of prime p in n) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Computes the sum of the cubes of the unitary divisors of n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety – not used for offset 1 */

    /* At most 16 distinct primes for any 64‑bit integer. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t exp3 = (idx_t)3 * exps[i];          /* 3*k */
        seq_t term = seq_pow((seq_t)primes[i], exp3);
        term = term + (seq_t)1;                  /* 1 + p^(3k) */
        result = seq_mul(result, term);
    }
    return result;
}
