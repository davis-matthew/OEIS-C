/*
 * A059289  a(n) = 1 + (binomial(2n,n) mod n).
 * https://oeis.org/A059289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (binomial(2*n, n) mod n)
 * Range:     u8 n <= 304, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   v_p_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/v_p_factorial.h"

/* Helper to compute power modulo a divisor (mod) */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (seq_t)(((uint64_t)result * base) % mod);
        base = (seq_t)(((uint64_t)base * base) % mod);
        exp >>= 1;
    }
    return result;
}

/* Prime table that grows as needed */
static idx_t *prime_tab = NULL;
static idx_t prime_cap  = 0;   /* maximal number sieved so far */
static size_t prime_cnt = 0;   /* number of primes stored */

/* Expand prime table up to at least "limit" (inclusive) */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_cap) return;

    size_t sz = (size_t)(limit + 1);
    bool *is_comp = (bool *)malloc(sz * sizeof(bool));
    if (!is_comp) return;                    /* rare failure: silently use old table */

    for (size_t i = 0; i < sz; ++i) is_comp[i] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (!is_comp[i]) ++cnt;

    idx_t *new_tab = (idx_t *)realloc(prime_tab, cnt * sizeof(idx_t));
    if (!new_tab) {
        free(is_comp);
        return;  /* keep old table */
    }
    prime_tab = new_tab;
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) prime_tab[idx++] = (idx_t)i;

    prime_cap = limit;
    prime_cnt = cnt;
    free(is_comp);
}

/* Legendre exponent of p in n! */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;      /* special case: modulo 1 is trivial */

    idx_t limit = 2 * n;
    ensure_primes(limit);

    seq_t mod = (seq_t)n;
    seq_t result = 1 % mod;           /* product of terms modulo n */

    for (size_t i = 0; i < prime_cnt && prime_tab[i] <= limit; ++i) {
        idx_t p = prime_tab[i];
        idx_t exp = v_p_factorial(limit, p) - 2 * v_p_factorial(n, p);
        if (exp) {
            seq_t part = mod_pow((seq_t)(p % n), (uint64_t)exp, mod);
            result = (seq_t)(((uint64_t)result * part) % mod);
        }
    }

    return (seq_t)(result + 1);   /* a(n) = 1 + (C(2n,n) mod n) */
}
