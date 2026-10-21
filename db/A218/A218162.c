/*
 * A218162  a(n) is the smallest positive integer k such that k^16 + 1 == 0
 *          mod p, where p is the n-th prime of the form 1 + 32*b (see
 *          A133870(n)).
 * https://oeis.org/A218162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest positive integer k such that k^16 + 1 == 0
 *            mod p, * where p is the n‑th prime of the form 1 + 32*b
 *            (A133870)
 * Range:     u8 n <= 2, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   factor_prime, is_prime, mod_pow, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/seq_cmp.h"    /* int seq_cmp(const void *p1, const void *p2) */
#include "../utils/factor_prime.h"

/* Find a primitive root modulo p (p > 2). */
static seq_t primitive_root(uint64_t p)
{
    uint64_t factors[64];
    size_t cnt, i;

    factor_prime(p - 1, factors, &cnt);

    for (uint64_t g = 2;; ++g) {
        bool ok = true;
        for (i = 0; i < cnt; ++i) {
            if (mod_pow((seq_t)g, (p - 1) / factors[i], (seq_t)p) == (seq_t)1) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)g;
    }
}

/* Compute the smallest k for a given prime p ≡ 1 mod 32. */
static seq_t smallest_k(uint64_t p)
{
    /* primitive root g of p */
    seq_t g = primitive_root(p);

    uint64_t t    = (p - 1) / 32;   /* exponent for first solution */
    uint64_t step = (p - 1) / 16;   /* difference between consecutive solutions */

    seq_t min_k = mod_pow(g, t, (seq_t)p);
    seq_t cur;
    for (uint64_t i = 1; i < 16; ++i) {
        cur = mod_pow(g, t + i * step, (seq_t)p);
        if (seq_cmp(&cur, &min_k) < 0) min_k = cur;
    }
    return min_k;
}

/* Public OEIS function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n ≤ 0 */

    /* static state for memoisation */
    static uint64_t *primes   = NULL;   /* primes of the form 32*b + 1 */
    static size_t  p_cap     = 0;
    static size_t  p_cnt     = 0;

    static seq_t *values    = NULL;   /* a(n) values */
    static size_t  v_cap    = 0;
    static size_t  v_cnt    = 0;

    static uint64_t next_candidate = 33;   /* 1 + 32*1; 1 itself is not prime */

    /* Generate required terms up to index n */
    while (v_cnt < (size_t)n) {
        /* Find next prime of the required form */
        while (!is_prime(next_candidate)) next_candidate += 32;
        uint64_t p = next_candidate;

        /* Grow primes array if necessary */
        if (p_cnt + 1 > p_cap) {
            size_t new_cap = p_cap ? p_cap << 1 : 16;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;
            primes = tmp;
            p_cap = new_cap;
        }
        primes[p_cnt++] = p;

        /* Compute a(n) for this prime */
        seq_t val = smallest_k(p);

        /* Grow values array if necessary */
        if (v_cnt + 1 > v_cap) {
            size_t new_cap = v_cap ? v_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            values = tmp;
            v_cap = new_cap;
        }
        values[v_cnt++] = val;

        /* Prepare for next candidate of form 32*b+1 */
        next_candidate += 32;
    }

    return values[n - 1];
}
