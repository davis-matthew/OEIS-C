/*
 * A098836  Deficient Smith numbers.
 * https://oeis.org/A098836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 762, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method:  Brute‑force search, accumulating composite
   numbers whose digit sum equals the digit sum of their prime
   factors (Smith condition) and whose sum of proper divisors is
   less than the number (deficient condition).  The function
   stores found values in a static cache so that arbitrary
   order of calls is supported. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

/* --- global state ----------------------------------------------------- */
static seq_t *cache = NULL;     /* memoised sequence values          */
static size_t cache_len = 0;    /* number of values stored           */
static size_t cache_cap = 0;    /* capacity of cache array           */
static uint64_t last_checked = 3; /* last number tested (inclusive)  */

/* --- helper: sum of decimal digits of a 64‑bit integer ---- */
static unsigned digit_sum_uint64(uint64_t x)
{
    unsigned s = 0;
    while (x > 0) {
        s += (unsigned)(x % 10);
        x /= 10;
    }
    return s;
}

/* --- helper: expand cache when necessary ---------------------------- */
static void cache_expand(void)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* --- helper: factorise n into primes and exponents ----------------- */
static void factor_uint64(uint64_t n, uint64_t primes[32], int exp[32], size_t *cnt)
{
    uint64_t temp = n;
    *cnt = 0;
    while (temp > 1 && *cnt < 32) {
        uint64_t p = _smallest_prime_factor(temp);
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        primes[*cnt] = p;
        exp[*cnt]   = e;
        (*cnt)++;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, callers will not use 0 */

    /* extend cache until we have at least n elements */
    while (cache_len < (size_t)n) {
        uint64_t cand = ++last_checked;   /* candidate integer */

        /* factorise cand */
        uint64_t primes[32];
        int expv[32];
        size_t cnt;
        factor_uint64(cand, primes, expv, &cnt);

        /* skip primes (Smith numbers must be composite) */
        if (cnt == 1 && expv[0] == 1) continue;

        /* digit sum of cand */
        unsigned sum_digits_n = digit_sum_uint64(cand);

        /* digit sum of prime factors (with multiplicity) */
        unsigned sum_digits_factors = 0;
        for (size_t i = 0; i < cnt; ++i) {
            sum_digits_factors += digit_sum_uint64(primes[i]) * expv[i];
        }

        /* not a Smith number */
        if (sum_digits_n != sum_digits_factors) continue;

        /* compute σ(n) using seq_t arithmetic to avoid overflow */
        seq_t sigma = (seq_t)1;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t sum_p = (seq_t)1;          /* includes p^0 */
            seq_t p_pow = (seq_t)1;
            uint64_t p = primes[i];
            int e = expv[i];
            for (int j = 0; j < e; ++j) {
                p_pow *= (seq_t)p;
                sum_p  += p_pow;
            }
            sigma *= sum_p;
        }

        /* sum of proper divisors = σ(n) – n */
        seq_t sum_proper = sigma - (seq_t)cand;

        /* not deficient */
        if (sum_proper >= (seq_t)cand) continue;

        /* candidate is a deficient Smith number */
        cache_expand();
        cache[cache_len++] = (seq_t)cand;
    }

    return cache[n - 1];
}
