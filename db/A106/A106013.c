/*
 * A106013  Primes p such that 80*p +- 3 are primes.
 * https://oeis.org/A106013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that 80*p‑3 and 80*p+3 are prime
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 15, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   add_seq, isprime_big, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/simple_sieve.h"   /* simple_sieve(uint32_t limit, uint32_t **primes_out, size_t *count_out); */
#include "../utils/isprime_big.h"    /* bool isprime_big(seq_t n); */
#include "../utils/add_seq.h"        /* void add_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val); */

/* Cached data for the sequence. */
static uint32_t *prime_arr   = NULL;   /* list of primes produced by simple_sieve   */
static size_t   prime_cnt   = 0;       /* number of primes in prime_arr             */
static size_t   prime_idx   = 0;       /* index of the next prime to test           */
static uint32_t sieve_limit = 1000000; /* current upper bound of the sieve          */

static seq_t  *seq_vals = NULL; /* array of the sequence values (good primes) */
static idx_t  seq_cap  = 0;     /* current capacity of seq_vals              */
static idx_t  seq_size = 0;     /* number of values stored in seq_vals       */

/* Ensure that seq_vals contains at least n entries. */
static void ensure_sequence(idx_t n)
{
    if (seq_size >= n)
        return;

    /* Generate primes in batches, doubling the limit each time we run out of good primes. */
    while (seq_size < n)
    {
        /* If we have no prime list yet, or we finished testing the current list, generate a new one. */
        if (prime_arr == NULL || prime_idx >= prime_cnt)
        {
            if (prime_arr != NULL) {
                free(prime_arr);
                prime_arr = NULL;
            }
            simple_sieve(sieve_limit, &prime_arr, &prime_cnt);
            prime_idx = 0;
        }

        /* Test the remaining primes in the current batch. */
        while (prime_idx < prime_cnt && seq_size < n)
        {
            seq_t p = (seq_t)prime_arr[prime_idx++];
            /* Compute 80*p‑3 and 80*p+3 */
            seq_t a = p * (seq_t)80 - (seq_t)3;
            seq_t b = a + (seq_t)6; /* 80p+3 = 80p‑3 + 6 */

            if (isprime_big(a) && isprime_big(b))
                add_seq(&seq_vals, &seq_cap, &seq_size, p);
        }

        if (seq_size < n) {
            /* Need more primes – double the sieve limit and loop again. */
            sieve_limit *= 2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n<1; returning 0 avoids UB. */

    ensure_sequence(n);
    return seq_vals[n - 1];
}
