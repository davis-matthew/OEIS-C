/*
 * A084374  Numbers k such that the smallest digit of k = number of prime
 *          divisors of k (counted with multiplicity).
 * https://oeis.org/A084374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 7856, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A084374: numbers k such that the smallest
   decimal digit of k equals the number of prime divisors of k
   (counted with multiplicity).  The function searches upward, tests
   each integer, and caches the good terms so that later calls are
   answered instantly.  The sequence is 1‑based (a(1) is the first term
   listed). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The build supplies these types.  They are used exactly as provided. */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Prime sieve – generates all primes up to PRIME_LIMIT.  The sieve
   is performed once, on first use.  The prime list is stored in a
   dynamic array that is grown if necessary.                           */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 2000000U   /* enough for the first ~10000 terms */

static uint32_t *prime_list = NULL;   /* primes, each < PRIME_LIMIT */
static size_t   prime_count = 0;
static size_t   prime_cap   = 0;

static void init_primes(void)
{
    if (prime_count > 0) return;         /* already initialized */

    bool *is_prime = (bool *)calloc((size_t)PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) return;               /* allocation failure – unlikely */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    if (PRIME_LIMIT >= 0) is_prime[0] = false;
    if (PRIME_LIMIT >= 1) is_prime[1] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Allocate space for the primes; approximate count: 2e6 / ln(2e6) ≈ 148933 */
    prime_cap = 200000;
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) {
        free(is_prime);
        return; /* unlikely */
    }

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) { free(is_prime); return; }
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint32_t)i;
        }
    }

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Count prime divisors with multiplicity (Ω(n)).                       */
/* ------------------------------------------------------------------ */
static unsigned count_omega(idx_t n)
{
    unsigned cnt = 0;
    idx_t temp = n;
    for (size_t i = 0; i < prime_count && temp > 1; ++i) {
        idx_t p = (idx_t)prime_list[i];
        if (p * (idx_t)p > temp) break;
        while (temp % p == 0) {
            ++cnt;
            temp /= p;
        }
    }
    if (temp > 1) ++cnt;   /* remaining prime factor */
    return cnt;
}

/* ------------------------------------------------------------------ */
/* Smallest decimal digit of n (0–9).  If n contains a zero digit,
   the result is 0.                                                   */
/* ------------------------------------------------------------------ */
static unsigned smallest_digit(idx_t n)
{
    unsigned min = 10;
    while (n > 0) {
        unsigned d = n % 10;
        if (d < min) min = d;
        if (min == 0) break;
        n /= 10;
    }
    return min;
}

/* ------------------------------------------------------------------ */
/* Test whether k satisfies the defining condition.                    */
/* ------------------------------------------------------------------ */
static bool good(idx_t k)
{
    unsigned d = smallest_digit(k);
    if (d == 0) return false;            /* zero digit cannot match positive count */
    return count_omega(k) == d;
}

/* ------------------------------------------------------------------ */
/* Cache of found terms of the sequence.                               */
/* ------------------------------------------------------------------ */
static seq_t  *cache      = NULL;
static idx_t   cache_size = 0;
static idx_t   cache_cap  = 0;

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static idx_t last_checked = 1;   /* last integer examined */

    init_primes();

    while (cache_size < n) {
        if (good(last_checked)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure – unlikely */
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
        ++last_checked;
    }

    return cache[n - 1];
}
