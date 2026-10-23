/*
 * A008913  Order of simple Chevalley group F_4(q), q = prime power.
 * https://oeis.org/A008913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^24 * (q^12-1) * (q^8-1) * (q^6-1) * (q^2-1), where q is
 *            the n‑th prime power
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 4, big any n
 * Helpers:   is_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/seq_pow.h"

static idx_t     next_candidate = 2;
static idx_t*    prime_list     = NULL;
static size_t    prime_cap      = 0;
static size_t    prime_cnt      = 0;

static seq_t*    cache          = NULL;
static size_t    cache_cap      = 0;
static size_t    cache_cnt      = 0;

static void add_prime(idx_t p)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        idx_t* newp = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!newp) abort();
        prime_list = newp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap < needed) new_cap <<= 1;
        seq_t* newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        cache = newp;
        cache_cap = new_cap;
    }
}

/* Checks whether m is a prime power; if exp_out is non‑NULL, stores the exponent. */
static int is_prime_power(idx_t m, int* exp_out)
{
    if (is_prime(m)) {
        if (exp_out) *exp_out = 1;
        return 1;
    }
    idx_t temp = m;
    idx_t first = 0;
    int exp = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if ((seq_t)p * (seq_t)p > temp) break;
        if (temp % p == 0) {
            first = p;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            break;
        }
    }
    if (first && temp == 1) {
        if (exp_out) *exp_out = exp;
        return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (cache_cnt < n) {
        idx_t m = next_candidate++;
        if (is_prime(m)) add_prime(m);
        int dummy_exp;
        if (!is_prime_power(m, &dummy_exp)) continue;
        seq_t q = (seq_t)m;
        seq_t val = seq_pow(q, 24);
        val *= (seq_pow(q, 12) - (seq_t)1);
        val *= (seq_pow(q, 8)  - (seq_t)1);
        val *= (seq_pow(q, 6)  - (seq_t)1);
        val *= (seq_pow(q, 2)  - (seq_t)1);
        ensure_cache_cap(cache_cnt + 1);
        cache[cache_cnt++] = val;
    }
    return cache[n - 1];
}
