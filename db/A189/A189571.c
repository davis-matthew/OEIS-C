/*
 * A189571  Smallest of nine consecutive primes whose sum is a prime.
 * https://oeis.org/A189571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A189571: Smallest prime in a block of nine consecutive primes whose sum is prime.
 * a(1) = 3, a(2) = 29, a(3) = 31, …
 *
 * Algorithm: generate primes one by one.  Whenever the list of primes
 * contains at least nine, compute the sum of the last nine primes.
 * If that sum is prime (checked with an uint64_t trial‑division routine),
 * record the first prime of that block.  Results are cached in a static
 * array so that arbitrary orders of calls are fast.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

static uint64_t *prime_arr = NULL;   /* list of primes (uint64_t) */
static size_t     prime_cap = 0;
static size_t     prime_len = 0;

static seq_t *result_arr = NULL;    /* cache of sequence terms */
static size_t  result_cap = 0;
static size_t  result_len = 0;

static uint64_t candidate = 5;      /* next odd candidate after 3 */

/* --------------------------------------------------------------------- */
static void prime_ensure(size_t needed)
{
    if (prime_cap < needed) {
        size_t newcap = prime_cap ? prime_cap << 1 : 8;
        if (newcap < needed) newcap = needed;
        uint64_t *tmp = (uint64_t *)realloc(prime_arr, newcap * sizeof(uint64_t));
        prime_arr = tmp;
        prime_cap = newcap;
    }
}

static void result_ensure(size_t needed)
{
    if (result_cap < needed) {
        size_t newcap = result_cap ? result_cap << 1 : 8;
        if (newcap < needed) newcap = needed;
        seq_t *tmp = (seq_t *)realloc(result_arr, newcap * sizeof(seq_t));
        result_arr = tmp;
        result_cap = newcap;
    }
}

static void append_prime(uint64_t p)
{
    prime_ensure(prime_len + 1);
    prime_arr[prime_len++] = p;
}

static void append_result(seq_t v)
{
    result_ensure(result_len + 1);
    result_arr[result_len++] = v;
}

/* --------------------------------------------------------------------- */
/* Trial division using primes we have already generated.
   This routine works for 64‑bit arguments and is called only for numbers
   up to ~10⁶, so it is fast enough. */
static bool is_prime_small(uint64_t n)
{
    if (n < 2)         return false;
    if (n == 2)        return true;
    if ((n & 1u) == 0u) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_arr[i];
        if (p > n / p)   break;
        if (n % p == 0)  return false;
    }
    return true;
}

/* 64‑bit primality test for the block‑sum.
   Uses the already known primes up to sqrt(n). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)           return false;
    if (n == 2)          return true;
    if ((n & 1u) == 0u)   return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_arr[i];
        if (p > n / p)      break;
        if (n % p == 0)     return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* The exported function required by the build system. */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (result_len >= (size_t)n)
        return result_arr[(size_t)n - 1];

    /* Initialise the prime list (only once). */
    if (prime_len == 0) {
        append_prime(2);
        append_prime(3);
        candidate = 5;
    }

    /* Generate primes and test nine‑prime blocks until n terms are found. */
    while (result_len < (size_t)n) {
        if (is_prime_small(candidate)) {
            append_prime(candidate);

            if (prime_len >= 9) {
                /* Sum the last nine primes (in uint64_t to avoid overflow). */
                uint64_t sum = 0;
                size_t start = prime_len - 9;
                for (size_t i = start; i < prime_len; ++i)
                    sum += prime_arr[i];

                /* If the sum itself is prime, record the first prime of the block. */
                if (is_prime_uint64(sum))
                    append_result(prime_arr[start]);
            }
        }
        candidate += 2;   /* skip even numbers */
    }

    return result_arr[(size_t)n - 1];
}
