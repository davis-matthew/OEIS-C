/*
 * A153145  Primes p such that 2*p + 19 is also prime.
 * https://oeis.org/A153145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that 2*p + 19 is also prime
 * Range:     u8 n <= 9, u16 n <= 422, u32 n <= 508, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Static storage for the generated sequence values */
static seq_t *seq_arr       = NULL;   /* dynamic array of sequence values */
static size_t seq_len       = 0;      /* number of values currently stored */
static size_t seq_cap       = 0;      /* current capacity of seq_arr    */

/* Static storage for the next candidate to test as a prime */
static uint64_t next_candidate = 2;   /* start from the smallest prime */

/* Helper to ensure that the sequence has at least n elements */
static void ensure_size(idx_t n)
{
    while (seq_len < n)
    {
        /* Find the next prime candidate */
        while (!is_prime_small((seq_t)next_candidate))
            ++next_candidate;

        /* Check if 2*p + 19 is also prime */
        seq_t val = (seq_t)next_candidate * (seq_t)2 + (seq_t)19;
        if (is_prime_big(val))
            append_candidate(&seq_arr, &seq_len, &seq_cap, (seq_t)next_candidate);

        /* Move to the next integer for the next iteration */
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; behaviour for n == 0 is undefined */
    ensure_size(n);
    return seq_arr[n - 1];
}
