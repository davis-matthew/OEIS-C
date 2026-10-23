/*
 * A128878  Primes of the form 47*n^2 - 1701*n + 10181.
 * https://oeis.org/A128878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the polynomial 47*k^2 - 1701*k + 10181,
 *            enumerated by increasing integer k (starting from k = 0)
 * Range:     u8 none, u16 n <= 27, u32 n <= 29, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of primes found so far */
    static seq_t *prime_cache      = NULL;   /* array of primes */
    static size_t  prime_count    = 0;        /* number of primes stored */
    static size_t  prime_capacity = 0;        /* allocated capacity */

    /* Next integer k to test in the polynomial */
    static idx_t   next_k          = 0;

    /* Return a dummy value for n = 0 (out of offset) */
    if (n == 0)
        return 0;

    /* Grow cache until we have at least n primes */
    while (prime_count < n)
    {
        /* Compute polynomial value in type seq_t to avoid overflow */
        seq_t k = (seq_t)next_k;
        seq_t val = 47 * k * k - 1701 * k + 10181;

        /* Only non‑negative values matter – polynomial is positive for k≥0 */
        if (val > 0)
        {
            /* For small seq_t types, skip values that would overflow the type */
            if (sizeof(seq_t) <= sizeof(uint64_t))
            {
                seq_t max_seq_t = ~ (seq_t)0;
                if (val > max_seq_t)
                {
                    next_k++;
                    continue;       /* value too large for seq_t */
                }
            }

            if (is_prime_big(val))
            {
                /* Ensure there is space for the new prime */
                if (prime_count == prime_capacity)
                {
                    size_t new_cap = (prime_capacity == 0) ? 8 : (prime_capacity << 1);
                    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                    prime_cache = tmp;
                    prime_capacity = new_cap;
                }
                prime_cache[prime_count++] = val;
            }
        }

        /* Advance k for next iteration */
        next_k++;
    }

    /* Return the requested prime, 1‑based index */
    return prime_cache[n - 1];
}
