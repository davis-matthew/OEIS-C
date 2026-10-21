/*
 * A054020  Chowla's function of n is not divisible by the number of proper
 *          divisors of n.
 * https://oeis.org/A054020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer m such that Chowla(m) = σ(m) –
 *            m – 1 is not divisible by the number of proper divisors of m
 *            (which equals τ(m) – 1)
 * Range:     u8 n <= 20, u16 n <= 1143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation stores the sequence values in a static
    cache and computes new terms on demand using trial
    division for factorisation.  It works for any sequence
    type seq_t (8/16/32/64/128 bits or arbitrary precision)
    and any idx_t type (typically uint64_t). */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ----- helper to compute integer powers using 128‑bit intermediates
   where available. --------------------------------------------------- */
static seq_t pow_uint128(uint64_t base, idx_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return (seq_t)result;
}

/* ----- cache for the computed sequence values ------------------------ */
static seq_t *cache  = NULL;       /* dynamic array of seq_t */
static size_t cache_len = 0;       /* number of values stored   */
static size_t cache_cap = 0;       /* allocated capacity        */

/* ----- current candidate integer to test ---------------------------- */
static uint64_t next_candidate = 2;

/* ----- main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n == 0 */

    /* compute terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t m = next_candidate;
        seq_t sigma = 1;                      /* sum of divisors of m */
        unsigned long long tau = 1;           /* divisor count of m  */
        uint64_t temp = m;
        uint64_t limit = (uint64_t)sqrt((double)temp);

        /* trial division factorisation */
        for (uint64_t p = 2; p <= limit && temp > 1; ++p) {
            if (temp % p == 0) {
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                seq_t p_pow = pow_uint128(p, exp + 1);
                seq_t sigma_factor = (p_pow - 1) / (p - 1);
                sigma *= sigma_factor;
                tau *= (unsigned long long)(exp + 1);
                limit = (uint64_t)sqrt((double)temp);
            }
        }
        /* handle last prime factor if remaining */
        if (temp > 1) {
            seq_t sigma_factor = (seq_t)temp + 1;
            sigma *= sigma_factor;
            tau *= 2;
        }

        /* compute Chowla value and proper divisor count */
        seq_t chowla = sigma - (seq_t)m - 1;
        unsigned long long proper = tau - 1;   /* exclude m itself */

        /* store m if Chowla(m) is not divisible by #proper divisors */
        if (proper != 0 && (chowla % (seq_t)proper) != 0) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               cache_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure – abort gracefully */
                    return 0;
                }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)m;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
