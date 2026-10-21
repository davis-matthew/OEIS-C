/*
 * A092902  Round(prime(n)/log[10](prime(n))).
 * https://oeis.org/A092902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Round(prime(n)/log_10(prime(n)))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>   /* used only for log10 in a double calculation */

/* Return the n‑th prime (n >= 1).  The primes are generated lazily
   and cached in a static array so that repeated calls are cheap.  */
static uint32_t *prime_list = NULL;   /* array of primes, 0‑based */
static size_t   prime_count  = 0;     /* number of primes stored */

/* Ensure that prime_list contains at least n primes (n >= 1).  */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    /* Allocate initial storage if necessary */
    if (prime_list == NULL) {
        size_t init = (n > 10 ? n : 10);   /* a rough estimate */
        prime_list = (uint32_t *)malloc(sizeof(uint32_t) * init);
        prime_count = 0;
    }

    /* Grow the array if needed */
    if (prime_count + 1000 > n) {
        size_t new_cap = prime_count + 1000;
        prime_list = (uint32_t *)realloc(prime_list, sizeof(uint32_t) * new_cap);
    }

    /* Start searching for new primes after the last stored one. */
    for (uint32_t candidate = (prime_count == 0 ? 2U : prime_list[prime_count - 1] + 1);
         prime_count < (size_t)n;
         ++candidate)
    {
        bool is_prime = true;
        uint32_t limit = (uint32_t)sqrt((double)candidate);
        for (size_t k = 0; k < prime_count; ++k) {
            uint32_t p = prime_list[k];
            if (p > limit)
                break;
            if ((candidate % p) == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_count++] = candidate;
        }
    }
}

/* The sequence function.  a(1) == 7, a(2) == 6, etc.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* undefined for n == 0, return 0 */

    /* get the n-th prime */
    ensure_primes(n);
    uint32_t p = prime_list[n - 1];

    /* compute rounded value of p / log10(p) */
    double val = (double)p / log10((double)p);
    int rounded = (int)(val + 0.5);   /* nearest integer, ties up */

    return (seq_t)rounded;
}
