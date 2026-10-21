/*
 * A091456  Least number k such that n * phi(k) < k, where phi is Euler's
 *          totient function.
 * https://oeis.org/A091456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the first t primes, where t is the minimal
 *            integer such that ∏_{i=1}^{t} (1 – 1/p_i) < 1/n
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Dynamically grown array of primes used by the routine */
static uint64_t *prime_list = NULL;
static size_t prime_list_cnt = 0;
static size_t prime_list_cap = 0;

/* Ensure the prime array can hold at least 'cap' entries */
static void ensure_prime_cap(size_t cap)
{
    if (prime_list_cap < cap) {
        size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 16;
        if (new_cap < cap) new_cap = cap;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();            /* allocation failure */
        prime_list = tmp;
        prime_list_cap = new_cap;
    }
}

/* Add a new prime to the list */
static void add_prime(uint64_t p)
{
    ensure_prime_cap(prime_list_cnt + 1);
    prime_list[prime_list_cnt++] = p;
}

/* Test whether a candidate is prime using the generated primes */
static int is_candidate_prime(uint64_t cand)
{
    for (size_t i = 0; i < prime_list_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > cand / p) break;   /* p*p > cand */
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* index < 1 is not used */

    seq_t num = 1;      /* ∏(p-1) */
    seq_t den = 1;      /* ∏p           */
    seq_t primorial = 1;/* ∏p           */
    seq_t sn = (seq_t)n;

    /* Generate primes and test the condition */
    for (uint64_t cand = 2; ; ++cand) {
        if (is_candidate_prime(cand)) {
            add_prime(cand);
            seq_t p = (seq_t)cand;

            num *= (p - 1);
            den *= p;
            primorial *= p;

            /* Check for n * num < den */
            if (sn * num < den) {
                return primorial;
            }
        }
    }
}
