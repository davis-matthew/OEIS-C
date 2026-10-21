/*
 * A092000  Numbers that can be expressed as the difference of the squares of
 *          primes in exactly four distinct ways.
 * https://oeis.org/A092000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092000: numbers that can be expressed as the difference of
   the squares of primes in exactly four distinct ways.  The function
   computes successive terms by sieving primes up to a dynamic limit
   and testing each even integer n (n ≡ 0 mod 4) for the required
   representation.  An O(n log log n) sieve and an O(n sqrt n) test
   per candidate yields all terms efficiently.  No hard‑coded table
   is used. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *terms = NULL;          /* array of found terms   */
static size_t terms_len = 0;         /* number of valid terms  */
static size_t terms_cap = 0;         /* allocated capacity     */
static uint32_t *spf = NULL;         /* smallest prime factor  */
static bool    *is_prime_bool = NULL;/* prime indicator       */
static size_t max_limit = 0;         /* current sieve limit    */

/* ------------------------------------------------------------------ */
/*  Simple integer square root for 32/64-bit values.  */
static uint32_t isqrt32(uint32_t n)
{
    uint32_t low = 0, high = n;
    while (low <= high) {
        uint32_t mid = (low + high) >> 1;
        uint64_t sq = (uint64_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else        high = mid - 1;
    }
    return high;
}

/* ------------------------------------------------------------------ */
/*  Build sieve up to new_limit.  Use linear sieve to get spf and
   is_prime_bool.  */
static void build_sieve(size_t new_limit)
{
    free(spf);
    free(is_prime_bool);
    spf = (uint32_t*)calloc(new_limit + 1, sizeof(uint32_t));
    if (!spf) abort();
    is_prime_bool = (bool*)calloc(new_limit + 1, sizeof(bool));
    if (!is_prime_bool) abort();

    size_t primes_est = new_limit / 10 + 1;
    uint32_t *primes = (uint32_t*)malloc(primes_est * sizeof(uint32_t));
    if (!primes) abort();
    size_t primes_len = 0;

    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_len >= primes_est) {
                primes_est <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, primes_est * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_len++] = i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || (uint64_t)i * p > new_limit)
                break;
            spf[i * p] = p;
        }
    }
    for (size_t i = 0; i < primes_len; ++i)
        is_prime_bool[primes[i]] = true;

    free(primes);
}

/* ------------------------------------------------------------------ */
/*  Rebuild all tables and find more terms up to new_limit.  */
static void rebuild_terms(size_t new_limit)
{
    free(spf);
    free(is_prime_bool);
    free(terms);
    spf = NULL;
    is_prime_bool = NULL;
    terms = NULL;
    terms_len = 0;
    terms_cap = 0;

    build_sieve(new_limit);
    max_limit = new_limit;

    /* estimate capacity (roughly one in ~1000 numbers) */
    terms_cap = new_limit / 1000 + 1;
    terms = (seq_t*)calloc(terms_cap, sizeof(seq_t));
    if (!terms) abort();

    for (uint32_t n = 4; n <= new_limit; n += 4) {
        size_t sqrt_n = isqrt32(n);
        int count = 0;
        for (size_t d = 2; d <= sqrt_n && count <= 4; ++d) {
            if (n % d) continue;
            uint32_t q = n / d;
            if (((d | q) & 1U) != 0) continue;          /* both must be even */
            uint32_t p = (d + q) >> 1;
            uint32_t r = (q - d) >> 1;
            if (p <= max_limit && r <= max_limit &&
                is_prime_bool[p] && is_prime_bool[r]) {
                ++count;
            }
        }
        if (count == 4) {
            if (terms_len >= terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)n;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Ensure that at least n terms are available. */
static void ensure_terms(idx_t n)
{
    if (terms_len >= (size_t)n) return;

    size_t new_limit = (max_limit == 0) ? 2000000 : max_limit << 1;
    while (terms_len < (size_t)n) {
        rebuild_terms(new_limit);
        if (terms_len >= (size_t)n) break;
        new_limit <<= 1;
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1 – behaviour below offset undefined */
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
