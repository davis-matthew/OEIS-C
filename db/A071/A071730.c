/*
 * A071730  Smallest prime p > prime(n) such that p-prime(n) is a square.
 * https://oeis.org/A071730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime p > prime(n) such that p - prime(n) is a
 *            square
 * Range:     u8 n <= 12, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple primality test for uint64_t values. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Dynamically grow a list of primes up to the requested index. */
static uint64_t *prime_list = NULL;
static size_t prime_count  = 0;
static size_t prime_cap    = 0;

static void ensure_primes(size_t need)
{
    if (prime_list == NULL) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_count = 0;
        prime_list[prime_count++] = 2;           /* seed with the first prime */
    }
    while (prime_count < need) {
        uint64_t candidate = prime_list[prime_count - 1] + 1;
        /* skip even numbers after 2 */
        if (candidate <= 2) candidate = 3;
        if ((candidate & 1) == 0) candidate++;

        while (!is_prime_uint64(candidate))
            candidate += 2;                      /* only odd candidates */

        if (prime_count == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_count++] = candidate;
    }
}

/* Return the n-th prime (1‑based index). */
static uint64_t nth_prime(idx_t n)
{
    ensure_primes((size_t)n + 4);   /* a small safety margin */
    return prime_list[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    uint64_t q = nth_prime(n);   /* current prime p_n */
    for (uint64_t k = 1;; ++k) {
        uint64_t diff = k * k;  /* square difference */
        uint64_t candidate = q + diff;
        if (is_prime_uint64(candidate))
            return (seq_t)candidate;
    }
}
