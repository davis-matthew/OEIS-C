/*
 * A160556  Positive integers b for which the Diophantine equation f = (b^(2n)
 *          - b^n + 8n^2 - 2) / (2n * (2n + 1)) has at least ten solutions for
 *          n <= 10000.
 * https://oeis.org/A160556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   mul_mod_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = n-th smallest positive integer b such that the Diophantine
 * equation (b^(2k) - b^k + 8k^2 - 2) / (2k*(2k+1)) is an integer
 * for at least ten distinct values of k with 1 <= k <= 10000.
 *
 * The function searches for such b lazily and caches the found values.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod_128.h"

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp != 0) {
        if (exp & 1)
            result = mul_mod_128(result, base, mod);
        base = mul_mod_128(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* --------- Criteria test for a given b -------------------------- */
#define N_LIMIT 10000ULL
#define REQUIRED 10ULL

static bool satisfies(const uint64_t b)
{
    uint64_t solutions = 0;
    for (uint64_t k = 1; k <= N_LIMIT && solutions < REQUIRED; ++k) {
        uint64_t two_k = 2 * k;
        uint64_t denom = two_k * (two_k + 1);          /* <= 4.0e8 */
        uint64_t p     = pow_mod_uint64(b, k, denom);   /* b^k mod denom */

        /* numerator modulo denom: (p^2 - p + 8k^2 - 2) mod denom */
        uint64_t term1 = 8 * k * k;                     /* 8k^2   */
        uint64_t temp  = mul_mod_128(p, p, denom);      /* p^2    */
        temp = (temp + denom - p) % denom;              /* p^2 - p */
        temp = (temp + term1 % denom + denom - 2) % denom;

        if (temp == 0)
            ++solutions;
    }
    return solutions >= REQUIRED;
}

/* --------- Cache of found b values -------------------------------- */
static seq_t *cache = NULL;      /* array of found b values            */
static size_t cache_size = 0;    /* number of stored b values          */
static size_t cache_cap = 0;     /* allocated capacity of the array    */
static uint64_t last_checked_b = 0;  /* last candidate examined          */

/* Ensure that at least 'limit' values are cached. */
static void ensure_cache(size_t limit)
{
    while (cache_size < limit) {
        ++last_checked_b;
        if (satisfies(last_checked_b)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* unrecoverable allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_checked_b;
        }
    }
}

/* -------------------- Public sequence function -------------------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    if (n == 0) return (seq_t)0;   /* undefined, return 0 as placeholder */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
