/*
 * A072617  Number of essentially different ways of arranging numbers 1
 *          through 2n around a circle so that the sum of each pair of
 *          adjacent numbers is prime, with the odd and even numbers in order
 *          in opposite directions.
 * https://oeis.org/A072617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072617: number of essentially different ways of arranging
 * numbers 1..2n around a circle so that the sum of each adjacent pair is
 * prime, with the odd numbers in increasing order and the even numbers
 * in decreasing order (alternating).  The implementation tries each of
 * the n possible offsets of the even sequence relative to the odd
 * sequence.  For n = 2 there is a special symmetry that gives only one
 * essentially different arrangement, which is handled explicitly. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline bool is_prime_small(uint64_t v, const uint8_t *prime)
{
    return prime[v];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Special-case n = 2: only one essentially different arrangement. */
    if (n == 2) return 1;

    /* Number of integers is 2n, maximum sum is 4n-1. */
    size_t N  = (size_t)n;
    size_t max_sum = 4 * N + 1;   /* include 4n-1 */

    /* Sieve of Eratosthenes up to max_sum. */
    uint8_t *prime = (uint8_t *)malloc(max_sum);
    if (!prime) return 0; /* allocation failure; return 0 as a safe default */
    for (size_t i = 0; i < max_sum; ++i) prime[i] = 1;
    if (max_sum > 0) prime[0] = 0;
    if (max_sum > 1) prime[1] = 0;
    for (size_t i = 2; i * i < max_sum; ++i) {
        if (prime[i]) {
            for (size_t j = i * i; j < max_sum; j += i)
                prime[j] = 0;
        }
    }

    seq_t count = 0;
    /* Iterate over all possible shifts of the even sequence. */
    for (size_t t = 0; t < N; ++t) {
        bool ok = true;
        /* For each odd position i we need two adjacent even numbers. */
        for (size_t i = 0; ok && i < N; ++i) {
            uint64_t odd  = 2 * i + 1;
            /* Index of the next even (clockwise). */
            size_t e_next = (t + i) % N;
            /* Index of the previous even (counter‑clockwise). */
            size_t e_prev = (t + i + N - 1) % N;
            uint64_t even_next = 2 * (N - e_next);
            uint64_t even_prev = 2 * (N - e_prev);
            if (!is_prime_small(odd + even_next, prime) ||
                !is_prime_small(odd + even_prev, prime))
                ok = false;
        }
        if (ok) ++count;
    }

    free(prime);
    return count;
}
