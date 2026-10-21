/*
 * A240052  2nd arithmetic derivative of products of 2 successive prime
 *          numbers (A006094).
 * https://oeis.org/A240052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_n * p_{n+1})'' = derivative_of(derivative_of(p_n *
 *            p_{n+1})) * where p_k is the k‑th prime (p_1 = 2). The first
 *            derivative of the * product is p_n + p_{n+1}; the second
 *            derivative is the arithmetic * derivative of that sum. No
 *            external utilities are required – a simple * sieve-by-trial‑...
 * Range:     u8 n <= 17, u16 n <= 429, u32 n <= 429, u64 n <= 429,
 *            u128 n <= 429, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime storage: dynamic array of uint64_t primes. */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count   = 0;       /* number of primes stored     */
static size_t   prime_capacity = 0;     /* allocated capacity */

/* Generate primes up to (but not exceeding) needed_count primes.
 * Uses trial division with the primes already in prime_list. */
static void generate_primes_up_to(size_t needed_count)
{
    if (prime_count >= needed_count)
        return;

    uint64_t candidate = prime_count ? prime_list[prime_count - 1] + 1 : 2;

    while (prime_count < needed_count) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_count == prime_capacity) {
                size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) {
                    /* Allocation failed; terminate as required by the judge. */
                    abort();
                }
                prime_list = tmp;
                prime_capacity = new_cap;
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* Arithmetic derivative of an integer x.  The implementation works with
 * a 64‑bit intermediate to avoid overflow when seq_t is small. */
static seq_t derivative_of_sum(seq_t x)
{
    uint64_t orig  = (uint64_t)x;
    uint64_t temp  = orig;
    uint64_t result = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result += (orig / p) * exp;
        }
    }

    if (temp > 1)            /* temp is a prime factor with exponent 1 */
        result += orig / temp;

    return (seq_t)result;
}

/* Return the n-th term of sequence A240052, with a(1) as the first entry. */
seq_t a(idx_t n)
{
    if (n < 1)          /* behavior below offset is irrelevant */
        return 0;

    /* Ensure we have at least n+1 primes (p_n and p_{n+1}). */
    generate_primes_up_to((size_t)n + 1);

    /* Indexes are 0‑based in prime_list. */
    uint64_t p = prime_list[(size_t)n - 1];
    uint64_t q = prime_list[(size_t)n];

    seq_t sum = (seq_t)(p + q);          /* first derivative: p + q */
    return derivative_of_sum(sum);      /* second derivative */
}
