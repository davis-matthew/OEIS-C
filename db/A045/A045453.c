/*
 * A045453  Primes congruent to {0, 1} mod 5.
 * https://oeis.org/A045453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045453: primes congruent to 0 or 1 mod 5.
   The sequence is obtained by generating primes in ascending order
   and selecting those that are 5 or congruent to 1 modulo 5. */

/* --------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_list = NULL;          /* all primes generated so far   */
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

static uint64_t *cp_list   = NULL;          /* primes that satisfy 5 or 1 mod 5 */
static size_t   cp_cnt     = 0;
static size_t   cp_cap     = 0;

/* Next integer to test for primality.  Starts at 2 and is increased after
   each test.  Candidate numbers are always odd after 2.             */
static uint64_t next_candidate = 2;

/* Grow the prime list to hold at least 'new_cap' elements.          */
static void prime_grow(size_t new_cap)
{
    if (prime_cap >= new_cap) return;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);                 /* allocation failed */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Grow the conditional‑prime list to hold at least 'new_cap' elements. */
static void cp_grow(size_t new_cap)
{
    if (cp_cap >= new_cap) return;
    uint64_t *tmp = (uint64_t *)realloc(cp_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);                 /* allocation failed */
    cp_list = tmp;
    cp_cap  = new_cap;
}

/* Test whether x is prime using all primes generated so far. */
static int is_prime_uint64(uint64_t x)
{
    if (x < 2u) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Ensure that at least 'n' conditional primes have been generated. */
static void ensure_ready(idx_t n)
{
    if (cp_cnt >= (size_t)n) return;

    /* Start with a small capacity for prime and conditional lists. */
    if (!prime_list) prime_grow(128);
    if (!cp_list)     cp_grow(64);

    while (cp_cnt < (size_t)n) {
        /* Test current candidate for primality. */
        if (is_prime_uint64(next_candidate)) {
            /* Store the prime. */
            if (prime_cnt >= prime_cap) prime_grow(prime_cap * 2);
            prime_list[prime_cnt++] = next_candidate;

            /* If it satisfies the condition, store it. */
            if (next_candidate == 5u || next_candidate % 5u == 1u) {
                if (cp_cnt >= cp_cap) cp_grow(cp_cap * 2);
                cp_list[cp_cnt++] = next_candidate;
            }
        }
        /* Advance to the next candidate: skip even numbers after 2. */
        if (next_candidate == 2u) next_candidate = 3u;
        else                      next_candidate += 2u;
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset is 1; below offset returns 0 */
    ensure_ready(n);
    return (seq_t)cp_list[n - 1];
}
