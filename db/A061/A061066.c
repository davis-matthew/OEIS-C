/*
 * A061066  a(n) = (prime(n)^2 - 1)/8.
 * https://oeis.org/A061066
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)^2 - 1)/8
 * Range:     u8 n <= 14, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* Forward declarations for internal helpers */
static void ensure_primes(idx_t n);

/* Static state for the prime list */
static size_t prime_count = 0;      /* number of primes stored */
static size_t prime_cap   = 0;      /* allocated size of the array */
static uint64_t *prime_list = NULL;

/* Ensure we have at least n primes (1‑based index).  */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    /* Estimate an upper bound for the nth prime (Rosser's theorem). */
    size_t limit;
    if ((size_t)n < 6)
        limit = 15;
    else {
        double dn = (double)n;
        limit = (size_t)(dn * (log(dn) + log(log(dn))) + 3.0);
    }

    /* Allocate a temporary sieve array.  */
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp)
        return; /* memory failure – caller will get an incorrect result */

    /* Grow the prime list as needed.  */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) {
            free(is_comp);
            return;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (count == prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!tmp) { /* allocation failure */
                    free(is_comp);
                    return;
                }
                prime_list = tmp;
            }
            prime_list[count++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    prime_count = count;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* behavior below offset is irrelevant */

    /* Make sure we have at least n primes stored. */
    ensure_primes(n);

    uint64_t p = prime_list[(size_t)(n - 1)]; /* prime(1) = 2 */
    seq_t pp = (seq_t)p;

    /* Compute (p^2 - 1)/8 using seq_t arithmetic. */
    return (pp * pp - 1) / 8;
}
