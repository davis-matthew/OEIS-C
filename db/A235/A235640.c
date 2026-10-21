/*
 * A235640  Primes p of the form n^2 + 1234567890 where 1234567890 is the
 *          first pandigital number with digits in order.
 * https://oeis.org/A235640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1956, u64 n <= 6694, u128 n <= 6694,
 *            big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Computes the nth prime of the form k^2 + 1234567890. */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Static state: dynamically grow the cache as needed. */
    static seq_t *cache = NULL;   /* array of primes found so far */
    static size_t len = 0;        /* number of stored primes */
    static size_t cap = 0;        /* capacity of the array */
    static idx_t next_k = 1;      /* next k to test */

    static const seq_t BASE = (seq_t)1234567890u;

    /* If we already have the answer, return it. */
    if (n <= len)
        return cache[n-1];

    /* Find more primes until we have at least n of them. */
    while (len < n) {
        seq_t k = (seq_t)next_k;
        seq_t p = BASE + k * k;
        if (isPrimeGeneric(p))
            append_candidate(&cache, &len, &cap, p);
        ++next_k;
    }
    return cache[n-1];
}
