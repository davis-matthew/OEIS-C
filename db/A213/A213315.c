/*
 * A213315  Numbers with exactly 8 nonprime substrings (substrings with
 *          leading zeros are considered to be nonprime).
 * https://oeis.org/A213315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5110, u32 n <= 7483, u64 n <= 7483,
 *            u128 n <= 7483, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: brute‑force search up to 10⁷,
   counting nonprime substrings (substrings that start with 0 are
   always counted as nonprime) to build OEIS A213315. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SIEVE_LIMIT 10000000U   /* proven upper bound for the sequence */

/* static state */
static bool  initialized = false;
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint8_t *is_prime = NULL;

/* initialise helper data on first call to a() */
static void init(void)
{
    if (initialized) return;

    /* -------- prime sieve up to SIEVE_LIMIT -------- */
    is_prime = (uint8_t *)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = 0;  is_prime[1] = 0;
    for (size_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t mult = p * p; mult <= SIEVE_LIMIT; mult += p)
                is_prime[mult] = 0;
        }
    }

    /* allocate cache for the list of terms */
    cache_cap = 1024;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);

    /* ---- scan all numbers up to the sieve limit ---- */
    unsigned int digits[10];   /* max 10 digits for numbers < 10¹⁰ */

    for (unsigned int x = 1011; x <= SIEVE_LIMIT; ++x) {
        /* obtain digits most‑significant first */
        unsigned int tmp = x;
        size_t len = 0;
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        for (size_t i = 0; i < len / 2; ++i) {
            unsigned int t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }

        /* count non‑prime substrings;
           substrings starting with 0 are always nonprime */
        size_t nonprime_cnt = 0;
        bool break_outer = false;
        for (size_t i = 0; i < len && !break_outer; ++i) {
            if (digits[i] == 0) {
                nonprime_cnt += (len - i);
                if (nonprime_cnt > 8) { break_outer = true; break; }
                continue;
            }
            unsigned int val = 0;
            for (size_t j = i; j < len; ++j) {
                val = val * 10 + digits[j];
                if (!is_prime[val]) {
                    ++nonprime_cnt;
                    if (nonprime_cnt > 8) { break_outer = true; break; }
                }
            }
        }

        if (!break_outer && nonprime_cnt == 8) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)x;
        }
    }

    initialized = true;
}

/* ---------------------------------------------------------- */
/* Public function required by the testing harness            */

seq_t a(idx_t n)
{
    init();                          /* make sure the cache is ready */

    if (n == 0) return 0;            /* undefined for offset 0 */
    if ((idx_t)n > (idx_t)cache_len) return 0;  /* out of bounds */

    return cache[(size_t)(n - 1)];
}
