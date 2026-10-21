/*
 * A059322  First differences of sequence of consecutive safe primes.
 * https://oeis.org/A059322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005385(n+1) - A005385(n) (difference of consecutive safe
 *            primes)
 * Range:     u8 n <= 10, u16 n <= 521, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* array of the generated safe primes (index 0 holds the first safe prime) */
    static seq_t *safe_primes = NULL;  /* array of safe primes */
    static size_t safe_cap   = 0;      /* allocated capacity (in seq_t elements) */
    static size_t safe_cnt   = 0;      /* number of stored safe primes */
    static seq_t cand        = 5;      /* next odd candidate to test */

    if (n == 0) return 0;              /* undefined for n < 1, return 0 */

    /* generate safe primes until we have at least n+1 of them */
    while (safe_cnt <= n)
    {
        seq_t p = cand;

        /* p is prime and (p-1)/2 is prime => p is a safe prime */
        if (is_prime((idx_t)p) && is_prime((idx_t)((p - 1) / 2)))
        {
            /* grow array if necessary */
            if (safe_cnt == safe_cap)
            {
                size_t new_cap = safe_cap ? safe_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(safe_primes, new_cap * sizeof(seq_t));
                if (!tmp) abort();              /* out of memory */
                safe_primes = tmp;
                safe_cap = new_cap;
            }
            safe_primes[safe_cnt++] = p;   /* store safe prime */
        }

        cand += 2;  /* next odd number */
    }

    /* difference between consecutive safe primes */
    return safe_primes[n] - safe_primes[n - 1];
}
