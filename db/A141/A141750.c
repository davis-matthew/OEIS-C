/*
 * A141750  Primes of the form 4*x^2 + 3*x*y - 4*y^2 (as well as of the form
 *          2*x^2 + 9*x*y + y^2).
 * https://oeis.org/A141750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime that can be expressed as 4*x^2 + 3*x*y -
 *            4*y^2 (equivalently 2*x^2 + 9*x*y + y^2). For odd primes p > 2,
 *            this holds iff the Legendre symbol (73|p) = 1; 2 and 73 are
 *            included explicitly. The algorithm tests primes sequentially and
 *            records those that satisfy the condition
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

/* Check whether a given odd prime p > 73 is representable.
   Must be called only when p has been confirmed prime. */
static bool rep_p(seq_t p)
{
    /* 2 and 73 are always represented. */
    if (p == (seq_t)2 || p == (seq_t)73)
        return true;

    /* Compute the Legendre symbol (73|p) by Euler's criterion:
       (73|p) ≡ 73^((p-1)/2) (mod p).  The result is 1, -1 (p-1), or 0. */
    seq_t exp_val = (p - (seq_t)1) >> 1;          /* (p-1)/2 as seq_t */
    uint64_t exp64 = (uint64_t)exp_val;          /* fits for the required range */

    seq_t leg = mod_pow((seq_t)73, exp64, p);
    return leg == (seq_t)1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of primes in the sequence   */
    static idx_t  size    = 0;      /* current number of cached terms    */
    static idx_t  capacity= 0;      /* current cache allocation         */
    static seq_t next_val = 2;      /* next integer to test             */

    /* Initialise cache on first call */
    if (!cache) {
        capacity = 8;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until we have at least n of them */
    while (size < n) {
        /* Stop if next_val wrapped to 0 (overflow) – only possible for tiny seq_t types. */
        if (next_val == (seq_t)0)
            break;

        if (is_prime_big(next_val) && rep_p(next_val)) {
            /* Grow cache if necessary */
            if (size + 1 > capacity) {
                capacity = capacity ? capacity * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[size++] = next_val;
        }
        ++next_val;
    }

    return cache[n - 1];   /* 1‑based indexing */
}
