/*
 * A187677  Primes of the form 8*k^2 + 6*k - 1 for positive k.
 * https://oeis.org/A187677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n^2 + 6*n - 1 for prime n, but implemented by testing
 *            the polynomial for successive k until the n‑th prime of that
 *            form is found
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache storing primes found so far. */
    static seq_t *primes   = NULL;   /* array of seq_t */
    static size_t primes_cnt = 0;    /* number of stored primes */
    static size_t primes_cap = 0;    /* current capacity of array */
    static idx_t next_k = 1;         /* next k to test */

    /* Undefined behavior for n == 0 – callers may assume >= 1. */
    if (n == 0)
        return 0;

    /* Allocate the cache on first use. */
    if (primes == NULL) {
        primes_cap = 16;
        primes = seq_alloc(primes_cap);
        if (primes == NULL)          /* out of memory – return failure. */
            return 0;
    }

    /* Generate candidates until we have at least n primes. */
    while (primes_cnt < n) {
        /* Compute candidate value: 8*k^2 + 6*k - 1 using idx_t to avoid
           overflow for typical inputs.  The result is small enough to
           cast to seq_t for storage and primality test. */
        idx_t k = next_k;
        idx_t val_i = 8 * k * k + 6 * k - 1;
        seq_t val = (seq_t)val_i;

        if (is_prime_big(val)) {
            /* Grow the array if necessary. */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
                if (tmp == NULL)
                    return 0;      /* allocation failure */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = val;
        }

        ++next_k;
    }

    return primes[n - 1];
}
