/*
 * A115710  Semiprimes (A001358) whose digit reversal is a square.
 * https://oeis.org/A115710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
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

#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return 0;   /* out‑of‑memory – abort (unlikely) */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = cand;   /* store the discovered term */
    }

    return cache[n - 1];
}
