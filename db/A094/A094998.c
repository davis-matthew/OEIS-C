/*
 * A094998  a(n) is smallest positive integer multiple of n-th prime, say
 *          k*prime(n), such that k*prime(n) == 1 (mod j) for each integer j
 *          with 1 < j < prime(n).
 * https://oeis.org/A094998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer multiple of the n‑th prime p
 *            such that the multiple ≡ 1 (mod j) for every j with 1 < j < p.
 *            The algorithm computes this by using the least common multiple
 *            of 1…p‑1. It works for any n ≥ 1 and memoises primes up to the
 *            largest prime required by successive calls. Th...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   _modular_inverse, big_pow, build_primes_upto, get_prime,
 *            powmod64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/get_prime.h"           /* uint32_t get_prime(idx_t k); */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/powmod64.h"            /* uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod); */
#include "../utils/big_pow.h"             /* seq_t big_pow(seq_t base, idx_t exp);   */
#include "../utils/seq_mul.h"             /* seq_t seq_mul(seq_t a, seq_t b);        */
#include "../utils/seq_pow.h"             /* seq_t seq_pow(seq_t base, idx_t exp);   */
#include "../utils/_modular_inverse.h"

/* Memoised primes up to the largest prime needed */
static uint64_t *memo_primes = NULL;
static size_t   memo_cnt   = 0;
static idx_t    memo_limit = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, behaviour for n<1 does not matter */

    uint32_t p = get_prime(n);                /* n-th prime, fits in uint32_t */
    uint64_t p64 = (uint64_t)p;               /* for 64‑bit arithmetic            */

    /* Special‑case n==1 to avoid zero‑length loop */
    if (p64 == 2) {
        return (seq_t)2;                      /* a(1) = 2 */
    }

    /* Ensure we have all primes up to p‑1 cached */
    if (p64 - 1 > memo_limit) {
        free(memo_primes);                    /* free old array if any */
        build_primes_upto(p64 - 1, &memo_primes, &memo_cnt);
        memo_limit = p64 - 1;
    }

    seq_t L   = (seq_t)1;                     /* current lcm as exact integer */
    uint64_t Lmod = 1;                        /* lcm modulo p for inverse     */

    /* Iterate over all primes q ≤ p‑1 */
    for (size_t i = 0; i < memo_cnt; ++i) {
        uint64_t q = memo_primes[i];
        if (q >= p64) break;

        /* Find largest e with q^e ≤ p‑1 */
        uint64_t pow = q;
        int e = 1;
        while (pow <= p64 - 1 && pow <= UINT64_MAX / q) {
            pow *= q;
            ++e;
        }
        if (pow > p64 - 1) --e;               /* adjust if overflow            */

        /* Multiply q^e into the exact LCM */
        seq_t term = seq_pow((seq_t)q, (idx_t)e);   /* q^e as seq_t */
        L = seq_mul(L, term);

        /* Update modulo value */
        uint64_t val_mod = powmod64(q, (uint64_t)e, p64);
        Lmod = (Lmod * val_mod) % p64;
    }

    /* Compute t = (-L⁻¹ mod p) mod p */
    int64_t invL = _modular_inverse((int64_t)Lmod, (int64_t)p64);
    int64_t t = (p64 - invL) % p64;
    seq_t t_seq = (seq_t)t;

    /* Result m = 1 + L * t */
    seq_t prod = seq_mul(L, t_seq);
    seq_t result = prod + (seq_t)1;

    return result;
}
