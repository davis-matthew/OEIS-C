/*
 * A085773  (n / product of digits of n) is a semiprime.
 * https://oeis.org/A085773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th positive integer m such that m /
 *            product_of_digits(m) is a semiprime
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   prod_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prod_digits.h"

static idx_t cache_len  = 0;
static idx_t cache_cap  = 0;
static seq_t *cache     = NULL;
static seq_t last_checked = 0;

/* Helper: true if n is a semiprime (product of exactly two primes). */
static bool is_semiprime(seq_t n)
{
    if (n < 2) return false;
    seq_t tmp = n;
    int factors = 0;

    /* check factor 2 */
    while ((tmp % 2) == 0) {
        tmp /= 2;
        if (++factors > 2) return false;
    }

    /* check odd factors */
    for (seq_t f = 3; f * f <= tmp; f += 2) {
        while (tmp % f == 0) {
            tmp /= f;
            if (++factors > 2) return false;
        }
    }
    if (tmp > 1) factors++;  /* remaining prime factor */

    return factors == 2;
}

/* Append value to cache, expanding as needed. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();      /* out of memory */
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        seq_t cand = last_checked + 1;
        last_checked = cand;

        seq_t prod = prod_digits(cand);
        if (prod == 0) continue;          /* has a zero digit -> skip */

        if ((cand % prod) != 0) continue; /* not divisible by product of digits */

        seq_t q = cand / prod;
        if (is_semiprime(q)) {
            append_cache(cand);
        }
    }
    return cache[n - 1];
}
