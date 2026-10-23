/*
 * A116974  Numbers n for which the sum of the proper divisors equals the
 *          product of the proper divisors.
 * https://oeis.org/A116974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116974: Numbers n for which the sum of proper divisors equals the product of proper divisors.
   The sequence is the set of all prime numbers together with 6 (which also satisfies the condition).  */

/* Returns the n-th term (n>=1).  */
/* a(1)=2, a(2)=3, a(3)=5, a(4)=6, a(5)=7, … */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static storage for generated primes */
static seq_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_cap = 0;
static uint64_t next_candidate = 2;    /* next integer to test for primality */

/* Append a prime value to the primes array, growing it if necessary */
static void add_prime(seq_t p)
{
    if (primes_count == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* fail fast */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_count++] = p;
}

/* Check whether x is a prime, using the primes already known.  x is cast to uint64_t. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    /* Already have primes less than sqrt(x) in the array?  */
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that at least ’need’ primes are available (primes_count >= need). */
static void ensure_primes_count(size_t need)
{
    if (primes_count >= need) return;

    /* Generate primes until we have enough */
    while (primes_count < need) {
        if (next_candidate == 2) {
            add_prime(2);
            next_candidate = 3;
            continue;
        }
        /* Only odd candidates beyond 2 */
        if (is_prime_uint64(next_candidate)) {
            add_prime((seq_t)next_candidate);
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* Main function requested by the build */
seq_t a(idx_t n)
{
    if (n < 1) abort();          /* undefined behaviour below offset, but guard */

    if (n == 4) {
        return (seq_t)6;
    }

    /* For n=1..3 or n>=5 we need primes */
    size_t prime_index = (n == 4) ? 0 : (n < 4 ? n : n - 1);

    ensure_primes_count(prime_index);

    return primes[prime_index - 1];
}
