/*
 * A162938  A 2-based alternate sum over the numbers from 0 to the n-th
 *          nonprime.
 * https://oeis.org/A162938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{x=0..k} (2 - x*(-1)^x), where k is the n‑th
 *            nonprime. The sum can be simplified to a closed form: if k is
 *            even: a(n) = (3*k + 4)/2 if k is odd : a(n) = (5*(k+1))/2 (k =
 *            sequence of nonprimes:
 *            0,1,4,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,28,30,31,32,33,35,…)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Cache of nonprime numbers (0 and 1 included). */
static uint64_t *nonprimes      = NULL;
static size_t   nonprimes_len  = 0;
static size_t   nonprimes_cap  = 0;

/* Ensure that the cache contains at least n nonprimers. */
static void
ensure_nonprimes(size_t n)
{
    while (nonprimes_len < n) {
        uint64_t candidate = (nonprimes_len == 0) ? 0
                               : nonprimes[nonprimes_len - 1] + 1;

        /* Skip primes; 0 and 1 are explicitly nonprime. */
        while (candidate >= 2 && is_prime((idx_t)candidate))
            ++candidate;

        /* Grow the vector if necessary. */
        if (nonprimes_len == nonprimes_cap) {
            size_t new_cap = (nonprimes_cap == 0) ? 64 : nonprimes_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(nonprimes, new_cap * sizeof(uint64_t));
            if (!tmp) {
                /* Allocation failure: return zero (undefined behavior otherwise). */
                exit(EXIT_FAILURE);
            }
            nonprimes      = tmp;
            nonprimes_cap  = new_cap;
        }

        nonprimes[nonprimes_len++] = candidate;
    }
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    ensure_nonprimes((size_t)n);

    uint64_t k = nonprimes[n - 1];   /* nth nonprime, zero‑based index */

    if ((k & 1ULL) == 0) {
        /* k is even */
        return ((seq_t)3 * (seq_t)k + (seq_t)4) / (seq_t)2;
    } else {
        /* k is odd */
        return ((seq_t)5 * (seq_t)(k + 1)) / (seq_t)2;
    }
}
