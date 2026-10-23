/*
 * A153039  Numbers k such that 2*k-7 is composite.
 * https://oeis.org/A153039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 2*k-7 is composite. */
/* Computes the nth term (offset 1). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------------- prime sieve ---------------------------------- */

#define INITIAL_PRIME_LIMIT 2
#define MAX_PRIMES 65536   /* enough for m up to ~10⁶ */

static uint32_t prime_list[MAX_PRIMES];
static size_t prime_cnt = 0;
static uint32_t prime_limit = 0;

/* generate primes up to new_limit (inclusive) */
static void ensure_primes_upto(uint32_t new_limit)
{
    if (new_limit <= prime_limit) return;

    for (uint32_t x = prime_limit + 1; x <= new_limit; ++x) {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > x) break;
            if (x % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_cnt < MAX_PRIMES) prime_list[prime_cnt++] = x;
        }
    }
    prime_limit = new_limit;
}

/* check primality using the prime list; works for uint32_t n */
static bool is_prime_uint32(uint32_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ----------------- sequence cache -------------------------------- */

static seq_t *cache = NULL;            /* array of computed terms   */
static size_t cache_cap = 0;           /* allocated capacity        */
static size_t cache_len = 0;           /* number of terms stored    */
static idx_t  next_k = 5;              /* next k to test (starts at 5) */

/* grow the cache to hold at least needed elements */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 32;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_buf;
    cache_cap = new_cap;
}

/* ----------------- sequence function ------------------------------ */

seq_t a(idx_t n)
{
    /* n >= 1  (undefined behaviour otherwise) */
    if (n == 0) return 0;

    /* initialise prime list once */
    if (prime_cnt == 0) {
        prime_limit = INITIAL_PRIME_LIMIT;
        prime_list[prime_cnt++] = 2;
    }

    /* make sure cache can store the desired element */
    ensure_cache_capacity((size_t)n);

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        idx_t k = next_k;
        ++next_k;                       /* advance for next iteration */

        /* compute m = 2*k - 7, as seq_t first */
        seq_t m = ((seq_t)2 * k) - 7;

        /* m must be > 1 and composite */
        if (m > 1U) {
            uint32_t m32 = (uint32_t)m; /* safe for the intended range */
            if (m32 > prime_limit) {
                ensure_primes_upto(m32);
            }
            if (!is_prime_uint32(m32)) {
                /* store k as a seq_t value */
                cache[cache_len++] = (seq_t)k;
            }
        }
    }

    return cache[n - 1];
}
