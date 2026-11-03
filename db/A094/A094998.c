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
 * Validated: standard, pass, 32 terms through n = 151 (2026-11-01)
 * Helpers:   build_primes_upto, ipow, ipow_idx, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"
#include "../utils/ipow_idx.h"
#include "../utils/powmod_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline int64_t
_modular_inverse(int64_t a, int64_t mod)
{
    int64_t b = mod, u = 1, v = 0;
    while (b) {
        int64_t t = a / b;
        int64_t tmp = a - t * b; a = b; b = tmp;
        tmp = u - t * v; u = v; v = tmp;
    }
    if (a != 1) return -1;             /* no inverse (should not happen)       */
    if (u < 0) u += mod;
    return u;
}

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
        seq_t term = ipow_idx((seq_t)q, (idx_t)e);   /* q^e as seq_t */
        L = seq_mul(L, term);

        /* Update modulo value */
        uint64_t val_mod = powmod_u64(q, (uint64_t)e, p64);
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
