/*
 * A063132  Composite numbers not divisible by 2 or 3 which in base 3 contain
 *          their largest proper factor as a substring.
 * https://oeis.org/A063132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th composite integer greater than 1 that is not
 *            divisible by 2 or 3 and whose base‑3 representation contains the
 *            base‑3 representation of its largest proper factor as a
 *            contiguous substring
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   to_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes terms by scanning the candidates 6k±1, testing
   compositeness, finding the largest proper factor, converting both
   numbers to base‑3, and checking the substring.  A static cache is
   kept so that every call to a(idx_t n) runs in O(n) time and
   subsequent calls are O(1).  Primes up to 1,000,000 are precomputed
   with a simple sieve for fast trial division.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/to_base3.h"

/* ---- Prime sieve up to 1,000,000 ---------------------------------- */
#define PRIME_LIMIT 1000000
static bool prime_sieve_done = false;
static unsigned int primes[80000];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_sieve_done) return;
    static bool is_composite[PRIME_LIMIT + 1];
    prime_cnt = 0;
    for (unsigned int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (unsigned int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    prime_sieve_done = true;
}

/* Checks if str2 is a substring of str1.  Both are null‑terminated. */
static bool contains_substr(const char *str1, const char *str2)
{
    if (!*str2) return true;   /* empty pattern matches */
    for (; *str1; ++str1) {
        const char *p1 = str1, *p2 = str2;
        while (*p2 && *p1 && *p1 == *p2) { ++p1; ++p2; }
        if (!*p2) return true; /* full match */
    }
    return false;
}

/* Finds the smallest prime factor of n.
   Returns true if n is composite; spf_out receives the factor.
   For prime numbers, returns false and spf_out is unchanged. */
static bool smallest_prime_factor(uint64_t n, unsigned *spf_out)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        unsigned p = primes[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) {
            *spf_out = p;
            return true;
        }
    }
    return false;  /* prime */
}

/* Checks if n (non‑prime, not divisible by 2 or 3) satisfies the property */
static bool qualifies(uint64_t n)
{
    /* ensure n is composite */
    unsigned spf = 0;
    if (!smallest_prime_factor(n, &spf)) return false;  /* prime, skip */

    /* largest proper factor L = n / spf */
    uint64_t L = n / spf;

    /* convert both numbers to base 3 strings */
    char buf_n[64], buf_L[32];
    int len_n, len_L;
    to_base3(n, buf_n, &len_n);
    to_base3(L, buf_L, &len_L);

    return contains_substr(buf_n, buf_L);
}

/* ---- Main sequence generator ------------------------------------- */
static uint64_t *term_cache = NULL;   /* cache of computed terms */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t next_k = 8;  /* k such that 6k+1 = 49 */
static int state = 0;        /* 0 -> generate 6k+1; 1 -> generate 6k+5 */

/* Retrieves the nth term (1‑based).  Lazy evaluation and caching. */
seq_t a(idx_t n)
{
    init_primes();  /* ensure primes are ready */

    /* grow cache as needed */
    while (cache_len < n) {
        uint64_t candidate;
        if (state == 0) {
            candidate = 6 * next_k + 1;
            state = 1;
        } else {
            candidate = 6 * next_k + 5;
            ++next_k;
            state = 0;
        }

        if (qualifies(candidate)) {
            /* append to cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap * 2) : 16;
                uint64_t *new_buf = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
                if (cache_cap) memcpy(new_buf, term_cache, cache_cap * sizeof(uint64_t));
                free(term_cache);
                term_cache = new_buf;
                cache_cap = new_cap;
            }
            term_cache[cache_len++] = candidate;
        }
    }
    return (seq_t)term_cache[n - 1];
}
