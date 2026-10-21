/*
 * A069899  Numbers k such that the integer quotient of largest and smallest
 *          prime factors of k is 1.
 * https://oeis.org/A069899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 7690, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A069899:
   numbers k such that the largest prime factor P(k) divided by
   the smallest prime factor p(k) satisfies floor(P(k)/p(k)) = 1,
   i.e. P(k) < 2·p(k).  The function generates terms in increasing
   order and caches them for future calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached list of terms */
static seq_t *term_cache   = NULL;
static size_t cache_len     = 0;   /* number of terms stored   */
static size_t cache_cap     = 0;   /* allocated capacity       */

/* Prime sieve (dynamic, only small primes needed) */
static uint64_t *prime_list = NULL;   /* primes in ascending order   */
static size_t   prime_cnt  = 0;       /* number of primes stored     */
static size_t   prime_cap  = 0;       /* allocated capacity          */

/* The largest k already examined; terms are generated up to max(last_k,
   needed k). */
static idx_t last_k = 1;             /* start at 1 (2 will be examined first) */

/* Ensure that prime_list contains all primes up to 'limit'.
   This routine grows the list one candidate at a time and tests for
   primality using already known primes, without calling sqrt().
   Complexity is negligible for the sizes involved (k < ~10⁶). */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit < 2) return;
    uint64_t start = (prime_cnt == 0 ? 2 : prime_list[prime_cnt - 1] + 1);
    if (start > limit) return;
    for (uint64_t cand = start; cand <= limit; ++cand) {
        bool isprime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap == 0 ? 8 : prime_cap * 2;
                uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = cand;
        }
    }
}

/* Append a term to the cache, enlarging it if necessary. */
static void cache_append(uint64_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        term_cache = tmp;
        cache_cap  = new_cap;
    }
    term_cache[cache_len++] = (seq_t)value;
}

/* Main sequence function.  It is safe to call this function with
   arbitrary (out‑of‑order) indices; terms are generated and cached on
   demand until the desired index is obtained. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n=0; return 0 as a placeholder */

    /* Return cached value if already computed */
    if ((size_t)n <= cache_len)
        return term_cache[(size_t)n - 1];

    /* Generate terms until we have at least n of them */
    for (uint64_t k = last_k + 1; (size_t)n > cache_len; ++k) {
        /* We need primes up to sqrt(k) for trial division. */
        uint64_t sqrt_k = 0;
        {
            uint64_t low = 0, high = k;
            while (low <= high) {
                uint64_t mid = (low + high) / 2;
                if (mid * mid <= k) { sqrt_k = mid; low = mid + 1; }
                else high = mid - 1;
            }
        }
        ensure_primes_upto(sqrt_k);

        uint64_t temp   = k;
        uint64_t smallest = 0;
        uint64_t largest  = 0;

        /* Trial division by known primes */
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > temp) break;
            if (temp % p == 0) {
                if (smallest == 0) smallest = p;
                largest = p;
                while (temp % p == 0) temp /= p;
            }
        }

        /* Remaining factor > 1 is prime */
        if (temp > 1) {
            if (smallest == 0) smallest = temp;
            if (temp > largest) largest = temp;
            /* Add the new prime to the list if it is new */
            if (prime_cnt == 0 || temp > prime_list[prime_cnt - 1]) {
                if (prime_cnt == prime_cap) {
                    size_t new_cap = prime_cap == 0 ? 8 : prime_cap * 2;
                    uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    prime_list = tmp;
                    prime_cap  = new_cap;
                }
                prime_list[prime_cnt++] = temp;
            }
        }

        /* Condition: largest < 2 * smallest  ⇔  largest/ smallest == 1 */
        if (largest / smallest == 1)
            cache_append(k);

        last_k = k;
    }

    return term_cache[(size_t)n - 1];
}
