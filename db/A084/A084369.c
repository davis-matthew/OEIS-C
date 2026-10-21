/*
 * A084369  Numbers n such that the n-th prime number doesn't contain any even
 *          digits.
 * https://oeis.org/A084369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   has_only_odd_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084369:  a(n) = the index k such that the k‑th prime contains only odd digits.
   The function generates primes incrementally, tests each for odd digits, and
   memoizes the resulting valid indices. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_only_odd_digits.h"

/* Prime handling */
static idx_t *prime_list  = NULL; /* Array of found primes            */
static size_t prime_cnt   = 0;     /* Number of primes stored          */
static size_t prime_cap   = 0;     /* Allocated capacity for prime_list*/
static idx_t  next_candidate = 2;   /* Next integer to test for primality */

/* Valid index handling */
static seq_t *valid_indices = NULL; /* Array of a(n) values           */
static size_t valid_cnt    = 0;      /* Number of a(n) values stored   */
static size_t valid_cap    = 0;      /* Allocated capacity for valid_indices */

/* Trial‑division test for primality using the primes already found. */
static bool is_prime(idx_t x)
{
    if (x < 2)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if ((p * p) > x)
            break;
        if ((x % p) == 0)
            return false;
    }
    return true;
}

/* Ensure that the prime array has at least 'needed' capacity. */
static void ensure_prime_cap(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap == 0 ? 8 : prime_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    prime_list = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    prime_cap  = new_cap;
}

/* Ensure that the valid index array has at least 'needed' capacity. */
static void ensure_valid_cap(size_t needed)
{
    if (valid_cap >= needed)
        return;
    size_t new_cap = valid_cap == 0 ? 8 : valid_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    valid_indices = (seq_t *)realloc(valid_indices, new_cap * sizeof(seq_t));
    valid_cap     = new_cap;
}

/* Main function: returns a(n). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0, defensive return */
        return (seq_t)0;

    /* Build up to the nth valid index if necessary. */
    while (valid_cnt < n) {
        /* Generate next prime. */
        while (true) {
            if (is_prime(next_candidate)) {
                /* Append to prime list. */
                ensure_prime_cap(prime_cnt + 1);
                prime_list[prime_cnt++] = next_candidate;

                /* Index of this prime is prime_cnt (1‑based). */
                if (has_only_odd_digits(next_candidate)) {
                    ensure_valid_cap(valid_cnt + 1);
                    valid_indices[valid_cnt++] = (seq_t)prime_cnt;
                    /* We have found one more valid index. */
                }

                ++next_candidate;
                break;  /* Move to the next iteration of outer loop. */
            }
            ++next_candidate;
        }
    }

    return valid_indices[n - 1];
}
