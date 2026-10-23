/*
 * A106715  Smallest of four consecutive primes whose sum of digits is prime.
 * https://oeis.org/A106715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest of the first in each set of four
 *            consecutive primes in which every prime’s digit sum is a prime
 *            number
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_digit_sum.h"

static uint32_t digit_sum(uint32_t x)
{
    uint32_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* ----------------- prime sieve and storage ----------------- */
static uint8_t *sieve       = NULL;
static size_t  sieve_limit  = 0;
static uint32_t *prime_list = NULL;
static size_t   prime_cap   = 0;
static size_t   prime_count = 0;

static void generate_primes(uint32_t limit)
{
    if (limit <= sieve_limit) return;

    /* free old sieve */
    free(sieve);
    sieve = (uint8_t *)malloc(limit + 1);
    memset(sieve, 0, limit + 1);
    sieve[0] = sieve[1] = 1;

    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* rebuild prime list */
    free(prime_list);
    prime_cap = limit / 10 + 10;        /* generous capacity */
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    prime_count = 0;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (prime_count >= prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = i;
        }
    }

    sieve_limit = limit;
}

/* ----------------- sequence storage ----------------- */
static seq_t *term_list       = NULL;
static size_t term_cap        = 0;
static size_t term_count      = 0;
static size_t term_scan_index = 0;   /* next prime index to start scanning */

/* compute terms until at least 'target' have been found */
static void compute_terms_until(size_t target)
{
    while (term_count < target) {
        /* scan quadruples of primes */
        for (size_t i = term_scan_index; i + 3 < prime_count; ++i) {
            uint32_t p[4] = {
                prime_list[i],
                prime_list[i + 1],
                prime_list[i + 2],
                prime_list[i + 3]
            };
            bool ok = true;
            for (int k = 0; k < 4 && ok; ++k)
                ok = is_prime_digit_sum(digit_sum(p[k]));

            if (ok) {
                if (term_count == term_cap) {
                    term_cap *= 2;
                    term_list = (seq_t *)realloc(term_list, term_cap * sizeof(seq_t));
                }
                term_list[term_count++] = (seq_t)p[0];
            }
            if (term_count >= target) {
                term_scan_index = i + 1;
                return;
            }
        }

        /* need more primes – double the limit */
        uint32_t new_limit = sieve_limit * 2;
        generate_primes(new_limit);
        /* term_scan_index is already at the next index to scan */
    }
}

/* ----------------- public function ----------------- */
static void init_once(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* pre‑sieve a generous bound (≈ 26 M) to obtain ~10 000 terms */
    generate_primes(26000000);

    term_cap = 20000; /* enough for typical requests */
    term_list = (seq_t *)malloc(term_cap * sizeof(seq_t));
    term_count = 0;
    term_scan_index = 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_once();

    if ((size_t)n > term_count)
        compute_terms_until((size_t)n);

    return term_list[(size_t)n - 1];
}
