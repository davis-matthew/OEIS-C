/*
 * A103960  Number of primes p such that prime(n)*p - 2 is prime and p <=
 *          prime(n).
 * https://oeis.org/A103960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Count primes p ≤ prime(n) such that prime(n)*p - 2 is also prime.
   The routine generates primes up to prime(n) on demand and performs a
   straightforward primality test using the generated list.  It works for
   any seq_t/idx_t combination supported by the build.  No hard‑coded data
   is used. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *prime_list = NULL;   /* dynamic array of primes */
static size_t    prime_count = 0;     /* number of primes stored */

/* ------------------------------------------------------------------ */
/* Simple primality test for numbers ≤ prime(n)*prime(n)-2.            */
/* ------------------------------------------------------------------ */
static bool isPrime64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return x == p;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Ensure that at least 'need' primes are stored in prime_list.       */
/* ------------------------------------------------------------------ */
static void ensure_primes(size_t need)
{
    if (prime_count >= need) return;

    /* Grow array if necessary. */
    size_t capacity = prime_count == 0 ? 16 : prime_count * 2;
    while (capacity < need) capacity *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, capacity * sizeof(uint64_t));
    if (!tmp) {
        /* Allocation failure – abort, as this is an internal error. */
        abort();
    }
    prime_list = tmp;

    /* Generate missing primes. */
    uint64_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    while (prime_count < need) {
        bool ok = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                ok = false;
                break;
            }
        }
        if (ok) prime_list[prime_count++] = candidate;
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n < 1 */

    /* Generate at least n primes. */
    ensure_primes((size_t)n);

    uint64_t prime_n = prime_list[n - 1];

    seq_t count = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] <= prime_n; ++i) {
        uint64_t p   = prime_list[i];
        uint64_t val = prime_n * p - 2;
        if (isPrime64(val)) ++count;
    }
    return count;
}
