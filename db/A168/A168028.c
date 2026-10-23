/*
 * A168028  Primes of the form 6n^2 - 342n + 4903.
 * https://oeis.org/A168028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n-1) where p(n) = 6*n^2 - 342*n + 4903 and p(n) is
 *            prime
 * Range:     u8 none, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) corresponds to p(0) */
    if (n == 0) return 0;            /* undefined, return 0 */

    /* static cache of primes found so far */
    static seq_t *primes = NULL;     /* array of found primes */
    static size_t len   = 0;         /* number of primes stored */
    static size_t cap   = 0;         /* capacity of the array */
    static idx_t  next_n = 0;        /* next polynomial index to test (starts at 0) */

    /* generate primes until we have at least n of them */
    while (len < (size_t)n) {
        /* compute the candidate value p(next_n) */
        seq_t val = 6 * (seq_t)next_n * (seq_t)next_n
                  - 342 * (seq_t)next_n
                  + 4903;

        /* test primality */
        if (is_prime_big(val)) {
            /* ensure capacity for another prime */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = seq_realloc(primes, cap, new_cap);
                if (!tmp) return 0;      /* allocation failure */
                primes = tmp;
                cap = new_cap;
            }
            primes[len++] = val;        /* store the prime */
        }
        ++next_n;                       /* move to next polynomial index */
    }

    return primes[n - 1];                /* 1‑based indexing */
}
