/*
 * A195814  Numbers k which use half of the ten digits such that they have at
 *          least one factorization k=p*q that uses remaining half of the
 *          digits that are not in k.
 * https://oeis.org/A195814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n‑th integer k (k≥1) that * uses exactly five
 *            distinct decimal digits and * admits a factorisation k = p * q
 *            such that * the five digits not in k are exactly the digits *
 *            appearing in p and q together (each exactly once)
 * Range:     u8 none, u16 n <= 19, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   digit_frequencies, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs an incremental search and
 * memoises found terms.  It uses only the helper
 * functions defined in the utils library (factor_u64
 * and digit_frequencies).  The algorithm tests each
 * candidate k (as a 64‑bit integer) for the required
 * digit property and for an admissible factorisation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/digit_frequencies.h"

/* Bit mask of the distinct digits in a 64‑bit integer.
   Bit i (0≤i≤9) is set iff digit i appears in x. */
static inline uint16_t digit_mask_uint64(uint64_t x)
{
    uint16_t m = 0;
    do {
        m |= 1u << (x % 10);
        x /= 10;
    } while (x);
    return m;
}

/* Returns true if the 64‑bit integer k satisfies the
 * A195814 condition. */
static bool is_valid_k(uint64_t k)
{
    /* k must use exactly five distinct decimal digits */
    uint16_t k_mask = digit_mask_uint64(k);
    if (__builtin_popcount((unsigned)k_mask) != 5)
        return false;

    /* Complement mask: the five digits not in k */
    const uint16_t ALL_DIGITS = 0x3FFu;     /* bits 0…9 */
    uint16_t comp_mask = (~k_mask) & ALL_DIGITS;

    /* Factorise k.  factor_u64 returns prime factors and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt;
    factor_u64(k, primes, exps, &pf_cnt);

    /* Enumerate all divisors of k.  We build them iteratively. */
    uint64_t divs[256];
    size_t divcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t oldcnt = divcnt;
        uint64_t p_pow = 1;
        for (uint64_t exp = 1; exp <= e; ++exp) {
            p_pow *= p;
            for (size_t j = 0; j < oldcnt; ++j) {
                divs[divcnt++] = divs[j] * p_pow;
            }
        }
    }

    /* Check all divisor pairs (p,q).  Since each divisor appears twice
     * (once as p, once as q when swapped), we can skip the divisor 1
     * and the divisor equal to k itself; but the tests are cheap. */
    for (size_t idx = 0; idx < divcnt; ++idx) {
        uint64_t p = divs[idx];
        if (p == 0 || p == k) continue;        /* skip trivial cases */
        uint64_t q = k / p;

        /* p and q must not contain any digit from k */
        uint16_t p_mask = digit_mask_uint64(p);
        uint16_t q_mask = digit_mask_uint64(q);
        if ((p_mask & k_mask) || (q_mask & k_mask))
            continue;

        /* p and q must use disjoint subsets of the complement digits */
        if ((p_mask & q_mask) != 0)
            continue;

        /* Union of digits of p and q must be exactly the complement set */
        if ((p_mask | q_mask) != comp_mask)
            continue;

        /* Ensure each complement digit appears exactly once across p and q.
         * We use digit_frequencies to count occurrences. */
        unsigned int freq_p[10] = {0};
        unsigned int freq_q[10] = {0};
        digit_frequencies(p, freq_p);
        digit_frequencies(q, freq_q);
        bool single_use = true;
        for (int d = 0; d < 10; ++d) {
            if ( (k_mask >> d) & 1u )      /* digit is in k -> skip */
                continue;
            int total = freq_p[d] + freq_q[d];
            if (total != 1) {               /* not exactly once */
                single_use = false;
                break;
            }
        }
        if (single_use)
            return true;
    }
    return false;
}

/* Memoised cache of found terms. */
static seq_t *cache     = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static uint64_t last_checked = 0;   /* last integer examined */

/* Append a new value to the cache, growing the array if needed. */
static inline void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 128 : cache_cap << 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; behaviour for n<1 is irrelevant */

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (is_valid_k(last_checked))
            cache_append((seq_t)last_checked);
    }
    return cache[n - 1];
}
