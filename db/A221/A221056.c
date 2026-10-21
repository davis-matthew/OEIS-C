/*
 * A221056  Numbers k such that there is no square between prime(k) and
 *          prime(k+1).
 * https://oeis.org/A221056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A221056: numbers k such that there is no square between
   prime(k) and prime(k+1).  The function keeps an internal cache
   of primes and of the required k-values so that calls can come in
   any order.  It uses trial division by previously found primes
   and an integer square‑root routine based on binary search.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer floor‑sqrt for unsigned 64‑bit values, O(log v) */
static idx_t _floor_isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t bit = 1ULL << 62;          /* start from the largest power of 4 <= max uint64 */
    idx_t res = 0;
    while (bit) {
        if (res + bit <= n) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defers offset handling, safe for non‑offset use */

    /* --- cache of primes --- */
    static idx_t  primes_cnt = 0;
    static idx_t  primes_cap = 0;
    static idx_t *primes     = NULL;   /* 64‑bit primes, stored as idx_t */

    /* --- cache of sequence terms (k's) --- */
    static idx_t  seq_cnt  = 0;
    static size_t seq_cap  = 0;
    static seq_t *seq      = NULL;      /* array of seq_t values */

    /* --- candidate for next odd number to test for prime --- */
    static idx_t candidate = 0;

    /* initialisation on first call */
    if (primes_cnt == 0) {
        primes_cap = 16;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        primes[0] = 2;
        primes_cnt = 1;
        candidate = 3;

        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate primes and evaluate the square‑gap condition until we have n terms */
    while (seq_cnt < n) {
        /* Find next prime > candidate */
        uint64_t p = candidate;
        while (true) {
            bool is_prime = true;
            /* trial division by known primes up to sqrt(p) */
            for (idx_t i = 0; i < primes_cnt; ++i) {
                idx_t q = primes[i];
                if ((uint64_t)q * (uint64_t)q > p) break;
                if (p % q == 0) { is_prime = false; break; }
            }
            if (is_prime) break;
            candidate += 2;              /* skip even numbers after 2 */
            p = candidate;
        }

        /* store the new prime */
        if (primes_cnt >= primes_cap) {
            size_t newcap = primes_cap << 1;
            if (newcap == 0) newcap = 16;
            primes_cap = newcap;
            primes = (idx_t *)realloc(primes, newcap * sizeof(idx_t));
        }
        primes[primes_cnt++] = (idx_t)p;
        candidate += 2;                  /* next odd candidate */

        /* if we have at least two primes, test the condition */
        if (primes_cnt >= 2) {
            idx_t prev_prime = primes[primes_cnt - 2];
            idx_t curr_prime = primes[primes_cnt - 1];
            idx_t sqrt_prev = _floor_isqrt_u64(prev_prime);
            idx_t sqrt_next_minus = _floor_isqrt_u64(curr_prime - 1);
            if (sqrt_prev == sqrt_next_minus) {
                /* store index of previous prime (1‑based) */
                if (seq_cnt >= seq_cap) {
                    size_t newcap = seq_cap << 1;
                    if (newcap == 0) newcap = 16;
                    seq_cap = newcap;
                    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                }
                seq[seq_cnt++] = (seq_t)(primes_cnt - 1);
            }
        }
    }

    return seq[n - 1];
}
