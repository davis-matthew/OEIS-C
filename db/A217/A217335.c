/*
 * A217335  Semiprimes p such that next semiprime after p is p+20.
 * https://oeis.org/A217335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217335 – semiprimes p such that the next semiprime after p is p+20 */
/* We generate semiprimes by trial‑division using a growing list of small primes.
   The function builds up a cache of terms and returns the n‑th term (1‑based). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: integer square root for 64‑bit unsigned integers */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n >> 1U | 1U;          /* hi ≥ sqrt(n) for n≥1 */
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1U);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) { lo = mid + 1; ans = mid; }
        else       hi = mid - 1;
    }
    return ans;
}

/* Growing list of primes for trial division */
static uint64_t *prime_list = NULL;   /* stored primes */
static size_t prime_len   = 0;        /* current count    */
static size_t prime_cap   = 0;        /* allocated space  */
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure prime_list contains all primes ≤ limit */
static void ensure_primes_upto(uint64_t limit)
{
    while (next_candidate <= limit) {
        bool prime = true;
        uint64_t sq = isqrt_u64(next_candidate);
        for (size_t i = 0; i < prime_len && prime_list[i] <= sq; ++i) {
            if (next_candidate % prime_list[i] == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_len == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* Determine whether n (≥4) is a semiprime (product of exactly two primes,
   counted with multiplicity).  It uses trial division by the primes table. */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;
    uint64_t temp = n;
    int count = 0;
    uint64_t limit = isqrt_u64(temp);
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_len && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        while (temp % p == 0) {
            ++count;
            temp /= p;
            if (count > 2) return false;
        }
        if (temp == 1) break;
    }
    if (temp > 1) ++count;
    return count == 2;
}

/* Cache of sequence values   (1‑based, stored at index[0] = a(1)) */
static seq_t *term_cache = NULL;   /* cache array */
static size_t term_len   = 0;      /* number of terms cached */
static size_t term_cap   = 0;      /* allocated capacity */

/* The last semiprime that was found (0 if none yet). */
static uint64_t last_semiprime = 0;

/* Candidate integer to test next for semiprimality (starts at 4). */
static uint64_t semip_candidate = 4;

/* Ensure that the cache contains at least 'need' terms. */
static void cache_at_least(size_t need)
{
    if (term_len >= need) return;

    while (term_len < need) {
        /* Find the next semiprime. */
        while (!is_semiprime(semip_candidate)) ++semip_candidate;

        /* Check gap to previous semiprime. */
        if (last_semiprime != 0 && semip_candidate - last_semiprime == 20) {
            /* Store last_semiprime in cache. */
            if (term_len == term_cap) {
                term_cap = term_cap ? term_cap * 2 : 16;
                term_cache = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
            }
            term_cache[term_len++] = (seq_t)last_semiprime;
        }

        /* Advance state. */
        last_semiprime = semip_candidate;
        ++semip_candidate;
    }
}

/* Public API – returns a(n) with 1‑based indexing.  Undefined for n<1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    cache_at_least((size_t)n);
    return term_cache[(size_t)n - 1];
}
