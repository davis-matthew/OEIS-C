/*
 * A060381  a(n) = prime(n)*prime(n+1)*...*prime(2*n-1), where prime(i) is the
 *          i-th prime.
 * https://oeis.org/A060381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * prime(n+1) * … * prime(2*n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code implements an incremental sieve of Eratosthenes and caches the
   generated primes so that repeated calls to a(idx_t n) are O(1) after
   the initial prime generation.  All operations use the user‑supplied
   types seq_t and idx_t and are fully defined for both C11 and C++17. */
 
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --- Prime storage ---------------------------------------------------- */
static unsigned int *prime_list  = NULL;   /* array of primes (1‑based indexing) */
static size_t        prime_count = 0;      /* number of primes stored      */
static size_t        prime_cap   = 0;      /* allocated capacity           */

/* Ensure that prime_list has at least nv primes.  This expands the array
   if necessary and runs a simple sieve to fill the missing primes. */
static void ensure_primes(idx_t nv)
{
    if (nv == 0) return;

    /* Enough primes already? */
    if (prime_count >= (size_t)nv) return;

    /* Estimate an upper bound for the nv-th prime (Rosser). */
    size_t limit;
    double n = (double)nv;
    if (nv < 6) {
        limit = 15;               /* 6th prime is 13 */
    } else {
        limit = (size_t)(n * (log(n) + log(log(n))) + 20.0);
    }

    /* Allocate/combine the prime array. */
    if (prime_cap < (size_t)nv) {
        size_t new_cap = (prime_cap == 0) ? 16 : prime_cap;
        while (new_cap < (size_t)nv) new_cap <<= 1;
        unsigned int *tmp = (unsigned int *)realloc(prime_list, new_cap * sizeof(unsigned int));
        if (!tmp) abort();           /* memory error */
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* Sieve up to 'limit' into prime_list. */
    unsigned char *is_composite = (unsigned char *)calloc(limit + 1, 1);
    if (!is_composite) abort();

    size_t cnt = 0;
    for (size_t i = 2; i <= limit && cnt < (size_t)nv; ++i) {
        if (!is_composite[i]) {
            prime_list[cnt++] = (unsigned int)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);
    prime_count = cnt;  /* should equal nv after the loop */
}

/* --- Sequence ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1u;    /* a(0) = 1 by definition */

    /* We need primes #n … #2n-1 (1‑based).  That is 2*n-1 primes. */
    ensure_primes(2 * n - 1u);

    seq_t prod = (seq_t)1u;
    for (idx_t k = n; k <= 2 * n - 1u; ++k) {
        prod *= (seq_t)prime_list[k - 1u];  /* prime_list[0] = prime(1) */
    }
    return prod;
}
