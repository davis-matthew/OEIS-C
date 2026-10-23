/*
 * A155471  Primes n with distinct smallest digits of n and n-th prime.
 * https://oeis.org/A155471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155471: primes n for which the smallest decimal digit of n and the
   smallest decimal digit of the n‑th prime are distinct. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the smallest decimal digit of a seq_t value. */
static seq_t smallest_digit(seq_t x)
{
    seq_t d = 9;
    while (x != 0) {
        seq_t t = x % 10;
        if (t < d) d = t;
        x /= 10;
    }
    return d;
}

/* Dynamic storage of primes as uint64_t values. */
static uint64_t *prime_list   = NULL;
static size_t    prime_cap    = 0;
static size_t    prime_count  = 0;
static uint64_t  next_candidate = 2;   /* next integer to test */

/* Grow the prime buffer. */
static void grow_prime_list(void)
{
    if (prime_cap == 0)
        prime_cap = 16;
    else
        prime_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    if (!tmp) abort();          /* safety */
    prime_list = tmp;
}

/* Generate primes until prime_count >= target. */
static void ensure_primes_up_to_count(size_t target)
{
    while (prime_count < target) {
        uint64_t c = next_candidate;
        if (c == 2)
            next_candidate = 3;
        else
            next_candidate += 2;      /* skip even candidates */

        bool is_prime = true;
        for (size_t j = 0; j < prime_count; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > c) break;
            if (c % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_count == prime_cap)
                grow_prime_list();
            prime_list[prime_count++] = c;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* outside offset */

    size_t found = 0;
    ensure_primes_up_to_count(1); /* have at least one prime */

    /* Iterate over primes in order until we find the nth desired one. */
    for (size_t idx = 0;; ++idx) {
        /* guarantee primes up to idx+1 */
        ensure_primes_up_to_count(idx + 1);

        uint64_t n_val = prime_list[idx];          /* current prime n */

        /* ensure we have prime(n_val) available */
        if (prime_count < (size_t)n_val)
            ensure_primes_up_to_count((size_t)n_val);

        seq_t nth_prime = (seq_t)prime_list[(size_t)n_val - 1];

        seq_t d1 = smallest_digit((seq_t)n_val);
        seq_t d2 = smallest_digit(nth_prime);

        if (d1 != d2) {
            ++found;
            if (found == n)
                return (seq_t)n_val;
        }
    }
}
