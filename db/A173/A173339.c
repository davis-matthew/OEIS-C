/*
 * A173339  Positive integers n for which the number of divisors of n^n is a
 *          square.
 * https://oeis.org/A173339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factor_u64, seq_alloc, seq_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A173339: Positive integers n for which the number of divisors of n^n is a square.
   This implementation searches integers in ascending order, caching the found terms.
   It uses factor_u64 from the utilities to factor 64‑bit integers. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_one.h"
#include "../utils/factor_u64.h"

/* helper: test whether tau(n^n) is a perfect square */
static bool is_square_divisor_tau(uint64_t n)
{
    /* factor n */
    uint64_t np[64], ne[64];
    size_t nc;
    factor_u64(n, np, ne, &nc);

    /* parity of prime exponents in the product ∏(ex_i * n + 1) */
    uint64_t primes[64];
    uint8_t par[64];
    size_t pcnt = 0;

    for (size_t i = 0; i < nc; ++i) {
        /* compute e_i * n + 1, careful about overflow */
        unsigned __int128 prod = (unsigned __int128)ne[i] * n + 1;
        if (prod > (unsigned __int128)UINT64_MAX) {
            /* overflow – we cannot factor the number: treat as not a square */
            return false;
        }
        uint64_t f = (uint64_t)prod;

        /* factor f */
        uint64_t fp[64], fe[64];
        size_t fc;
        factor_u64(f, fp, fe, &fc);

        /* accumulate parity */
        for (size_t j = 0; j < fc; ++j) {
            uint64_t p = fp[j];
            uint8_t epar = fe[j] & 1U;
            size_t k;
            for (k = 0; k < pcnt; ++k) {
                if (primes[k] == p) break;
            }
            if (k == pcnt) {
                primes[pcnt] = p;
                par[pcnt] = epar;
                ++pcnt;
            } else {
                par[k] ^= epar;
            }
        }
    }

    /* all parities must be even */
    for (size_t i = 0; i < pcnt; ++i) {
        if (par[i]) return false;
    }
    return true;
}

/* static cache of discovered terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;

seq_t a(idx_t n)
{
    if (n == 0) return seq_one();

    /* extend cache until it contains n terms */
    while (cache_len < (size_t)n) {
        ++last_checked;

        if (is_square_divisor_tau(last_checked)) {
            /* ensure cache has room for the new element */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp;
                if (cache == NULL)
                    tmp = seq_alloc(new_cap);
                else
                    tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) return seq_one();    /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
