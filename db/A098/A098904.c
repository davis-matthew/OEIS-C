/*
 * A098904  Even numbers whose number of distinct prime factors is odd.
 * https://oeis.org/A098904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) are even numbers with an odd number of distinct prime factors.
   The function builds the sequence by scanning through even integers,
   using a precomputed list of primes to count distinct prime factors. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

static seq_t *seq = NULL;          /* array of sequence values   */
static size_t seq_len = 0;          /* number of values generated */
static size_t seq_cap = 0;          /* allocated capacity         */
static idx_t next_candidate = 2;   /* next even number to test   */

static uint64_t *prime_list = NULL; /* primes up to PRIME_MAX    */
static size_t prime_cnt = 0;         /* number of primes          */
#define PRIME_MAX 1000000          /* enough for n <= 10000     */

/* Count distinct prime factors of m using the global prime list. */
static size_t distinct_prime_factor_count(idx_t m)
{
    size_t count = 0;
    idx_t temp = m;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0)
        {
            ++count;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) ++count; /* remaining prime factor */
    return count;
}

/* Ensure the prime list is computed once. */
static void init_prime_list(void)
{
    if (prime_list != NULL) return;
    build_primes_upto(PRIME_MAX, &prime_list, &prime_cnt);
}

/* Grow the sequence array to accommodate at least size elements. */
static void ensure_seq_capacity(size_t size)
{
    if (seq_cap >= size) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < size) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Generate sequence terms until we have at least n terms. */
static void generate_up_to(idx_t n)
{
    init_prime_list();
    while (seq_len < (size_t)n)
    {
        /* Candidate is always even */
        idx_t val = next_candidate;
        next_candidate += 2; /* next even number */

        size_t omega = distinct_prime_factor_count(val);
        if ((omega & 1) == 1)   /* odd number of distinct primes */
        {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */
    generate_up_to(n);
    return seq[n - 1];
}
