/*
 * A115710  Semiprimes (A001358) whose digit reversal is a square.
 * https://oeis.org/A115710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the sequence on demand.  It keeps a local
 * cache of computed terms and repeatedly tests consecutive integers for
 * two properties:
 *   1. it is a semiprime (product of two primes, possibly equal);
 *   2. the decimal-reversal of the integer is a perfect square.
 * The routine is memoised and independent of the order of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

/* --------------------------------------------------------------------- */
/*                                           Prime sieve (size-limited) */
/* --------------------------------------------------------------------- */

/* Rough upper bound on primes needed for the first 10 000 terms.
   20 000 000 comfortably exceeds the required range. */
#define PRIME_SIEVE_LIMIT 20000000U

static uint32_t *primes = NULL;
static size_t primes_count = 0;
static bool primes_initialised = false;

/* Initialise the prime list by an ordinary sieve. */
static void init_primes(void)
{
    if (primes_initialised) return;
    const size_t limit = PRIME_SIEVE_LIMIT;
    uint8_t *is_comp = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!is_comp) return;           /* out‑of‑memory → empty prime list */

    is_comp[0] = is_comp[1] = 1;
    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* Allocate space for all primes up to the limit. */
    primes = (uint32_t*)malloc((limit / 10 + 10) * sizeof(uint32_t));
    if (!primes) { free(is_comp); return; }

    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            primes[primes_count++] = (uint32_t)i;

    free(is_comp);
    primes_initialised = true;
}

/* --------------------------------------------------------------------- */
/*                                           Helper functions          */
/* --------------------------------------------------------------------- */

/* Reverse the decimal digits of x. Works for any seq_t supported by %10 and /10. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t d = x % 10;
        rev = rev * 10 + d;
        x = x / 10;
    }
    return rev;
}

/* Test whether x (seq_t) is a semiprime. */
static bool is_semiprime(seq_t x)
{
    if (x < 4) return false;    /* 4 = 2×2 is the smallest semiprime */

    init_primes();

    seq_t sqrt_x = floor_sqrt(x);

    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        seq_t sp = (seq_t)p;
        if (sp > sqrt_x) break;
        if (x % sp == 0) {
            seq_t q = x / sp;
            return is_prime_big(q);
        }
    }

    /* No small divisor found – maybe it is the square of a large prime. */
    if (sqrt_x * sqrt_x == x && is_prime_big(sqrt_x))
        return true;

    return false;
}

/* Both properties hold for x? */
static bool is_valid(seq_t x)
{
    seq_t rev = reverse_digits(x);
    seq_t r   = floor_sqrt(rev);
    if (r * r != rev) return false;
    return is_semiprime(x);
}

/* --------------------------------------------------------------------- */
/*                                           Main sequence function     */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached terms */
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_candidate = 4;  /* candidate to be tested next   */

    init_primes();

    while (cache_len < (size_t)n) {
        idx_t cand_idx = next_candidate++;
        seq_t cand = (seq_t)cand_idx;

        if (!is_valid(cand))
            continue;      /* skip non‑term */

        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* out‑of‑memory – abort (unlikely) */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = cand;   /* store the discovered term */
    }

    return cache[n - 1];
}
