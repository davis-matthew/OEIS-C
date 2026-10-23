/*
 * A161506  Number of divisors of n that are greater than phi(n)/2, where phi
 *          is Euler's totient function.
 * https://oeis.org/A161506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d > φ(n)/2
 * Range:     u8 n <= 9950, u16 n <= 9950, u32 n <= 9950, u64 n <= 9950,
 *            u128 n <= 9950, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the helper fn
 *   void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt);
 *  which must be provided by the build system. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Treat n < 1 as a special case – the offset is 1, but the value is
       undefined for n == 0.  Returning 0 here keeps the function total
       without influencing the sequence starting at n = 1. */
    if (n == 0)
        return 0;

    /* Work with 64‑bit arithmetic for the factorisation and phi
       computation.  All intermediate results fit into 64 bits
       because n ≤ 2^64‑1.  The final count is known to be small
       (≤ divisor count of n).  Casting to seq_t is safe. */
    uint64_t nn = (uint64_t)n;

    /* --- Factor n ----------------------------------------------------- */
    /* There are at most 64 distinct prime factors for a 64‑bit integer,
       so we allocate space for that many. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   num_primes;

    factor_u64(nn, primes, exps, &num_primes);

    /* --- Compute φ(n) ----------------------------------------------- */
    uint64_t phi = nn;
    for (size_t i = 0; i < num_primes; ++i) {
        /* phi = phi / p * (p-1) */
        phi = phi / primes[i] * (primes[i] - 1ULL);
    }

    /* --- Generate all divisors -------------------------------------- */
    /* 10000 has at most 128 divisors; 4096 is ample for all 64‑bit
       inputs. */
    uint64_t divisors[4096];
    size_t div_cnt = 1;          /* start with divisor 1 */
    divisors[0] = 1ULL;

    for (size_t i = 0; i < num_primes; ++i) {
        uint64_t p = primes[i];
        size_t   exp = (size_t)exps[i];
        size_t   old_cnt = div_cnt;
        for (size_t j = 0; j < old_cnt; ++j) {
            uint64_t base = divisors[j];
            for (size_t k = 1; k <= exp; ++k) {
                base *= p;
                divisors[div_cnt++] = base;
            }
        }
    }

    /* --- Count divisors greater than φ(n)/2 -------------------------- */
    uint64_t count = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        if (2ULL * divisors[i] > phi)
            ++count;
    }

    return (seq_t)count;
}
