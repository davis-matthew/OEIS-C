/*
 * A031376  a(n) = prime(10*n - 1).
 * https://oeis.org/A031376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(10*n - 1)
 * Range:     u8 n <= 5, u16 n <= 654, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for the list of primes generated so far. */
static uint64_t *primes = NULL;
static size_t    primes_count   = 0;  /* number of primes stored      */
static size_t    primes_capacity= 0;  /* allocated capacity           */

/* Add primes to the list until at least `need` primes are available. */
static void ensure_primes_upto(idx_t need)
{
    /* Grow the prime array if necessary. */
    if (primes_capacity < need)
    {
        size_t new_cap = primes_capacity ? primes_capacity * 2 : 128;
        while (new_cap < need)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) /* allocation failure – abort the program */
        {
            /* In a gsl context you would use abort(). */
            exit(EXIT_FAILURE);
        }
        primes       = tmp;
        primes_capacity = new_cap;
    }

    /* Current end of the prime list. */
    uint64_t last_checked = primes_count ? primes[primes_count - 1] : 1;
    /* Find the next prime candidate.  Start at 2 if the list is empty. */
    if (last_checked < 2) last_checked = 1;

    while (primes_count < need)
    {
        ++last_checked;
        /* Skip even numbers > 2. */
        if (last_checked > 2 && (last_checked & 1) == 0)
            continue;

        /* Test primality by trial division with primes we already know. */
        int is_prime = 1;
        for (size_t i = 0; i < primes_count; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > last_checked) /* no need to test larger primes */
                break;
            if (last_checked % p == 0)
            {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
        {
            primes[primes_count++] = last_checked;
        }
    }
}

/* Sequence function: returns the n‑th term of A031376. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n == 0 */

    idx_t k = 10 * n - 1;              /* desired prime index (1‑based) */
    ensure_primes_upto(k);

    /* primes[0] is the first prime, primes[1] the second, ... */
    return (seq_t)primes[k - 1];
}
