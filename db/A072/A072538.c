/*
 * A072538  Erroneous version of A000040 (the primes).
 * https://oeis.org/A072538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n <= 3) ? prime(n) : prime(n+1) (Offset 1)
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Dynamic array of all primes. */
static seq_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static seq_t current_candidate;

/* Ensure capacity for at least 'need' primes. */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    primes = tmp;
    prime_cap = new_cap;
}

/* Initialise the prime table with the first prime. */
static void init_primes(void)
{
    ensure_prime_cap(1);
    primes[0] = (seq_t)2;
    prime_cnt = 1;
    current_candidate = 3;
}

/* Generate primes until we have at least 'needed' primes. */
static void generate_primes_up_to(size_t needed)
{
    if (prime_cnt == 0)
        init_primes();
    if (prime_cnt >= needed)
        return;

    while (prime_cnt < needed) {
        /* Find the next prime. */
        while (true) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                seq_t p = primes[i];
                if ((unsigned long long)p * p > (unsigned long long)current_candidate)
                    break;
                if (current_candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;
            current_candidate += 2;   /* Skip even numbers. */
        }

        /* Store the found prime. */
        ensure_prime_cap(prime_cnt + 1);
        primes[prime_cnt++] = current_candidate;
        current_candidate += 2;       /* Next candidate is the following odd number. */
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; values below are unused */
        return 0;

    if (n <= 3) {
        generate_primes_up_to((size_t)n);
        return primes[n - 1];
    } else {
        generate_primes_up_to((size_t)(n + 1));
        return primes[n];
    }
}
