/*
 * A094317  Primes with the property that the sum of the digits that are
 *          primes is prime.
 * https://oeis.org/A094317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   prime_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/prime_digit_sum.h"

/* Implements OEIS A094317: primes whose sum of prime digits (2,3,5,7) is also prime. */

/* Small primality test for the digit‑sum (values < 256). */
static bool is_small_prime(int v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    if (v % 3 == 0) return v == 3;
    for (int i = 5; i * i <= v; i += 6) {
        if (v % i == 0 || v % (i + 2) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the offset 1 */

    /* ---------- static data ----------
       cache for the prime sieve, the filtered primes, and the result array */
    static bool *is_composite = NULL;
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static idx_t sieve_limit = 0;

    static seq_t *good = NULL;            /* array of numbers that satisfy the rule */
    static size_t good_cap = 0, good_len = 0;
    static idx_t prime_pos = 0;           /* next prime to examine */

    /* one‑time initialization: build a sieve up to the chosen limit */
    if (sieve_limit == 0) {
        sieve_limit = 200000;            /* safe for n up to 10000 */

        is_composite = (bool*)calloc((size_t)sieve_limit + 1, sizeof(bool));
        if (!is_composite) return (seq_t)0;

        /* simple sieve of Eratosthenes */
        prime_cnt = 0;
        for (idx_t i = 2; i <= sieve_limit; ++i) {
            if (!is_composite[i]) {
                ++prime_cnt;
                if (i * i <= sieve_limit) {
                    for (idx_t j = i * i; j <= sieve_limit; j += i)
                        is_composite[j] = true;
                }
            }
        }

        prime_list = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
        if (!prime_list) { free(is_composite); return (seq_t)0; }

        size_t k = 0;
        for (idx_t i = 2; i <= sieve_limit; ++i) {
            if (!is_composite[i])
                prime_list[k++] = (uint64_t)i;
        }

        free(is_composite);
        is_composite = NULL;
    }

    /* generate qualifying primes until we have at least n of them */
    while (good_len < (size_t)n) {
        if (prime_pos >= prime_cnt) break;     /* ran out of pre‑computed primes */

        uint64_t p = prime_list[prime_pos++];
        uint64_t s = prime_digit_sum(p);
        if (s == 0) continue;                 /* no prime digits */
        if (!is_small_prime((int)s)) continue;/* sum not prime */

        if (good_len == good_cap) {
            size_t new_cap = good_cap ? good_cap * 2 : 8;
            seq_t *tmp = (seq_t*)realloc(good, new_cap * sizeof(seq_t));
            if (!tmp) { free(prime_list); return (seq_t)0; }
            good = tmp;
            good_cap = new_cap;
        }
        good[good_len++] = (seq_t)p;
    }

    if (n <= good_len) return good[n - 1];
    return (seq_t)0;                     /* should not happen for the allowed range */
}
