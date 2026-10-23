/*
 * A023245  Primes that remain prime through 2 iterations of the function f(x)
 *          = 2x + 9.
 * https://oeis.org/A023245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 351, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   allocate_seq_array, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes p such that 2p+9 and 4p+27 are also primes.
   This implements OEIS A023245 by scanning primes and caching the
   qualifying ones. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/allocate_seq_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; behavior for n==0 is irrelevant. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;      /* cached qualifying primes */
    static size_t cap   = 0;          /* allocated capacity */
    static size_t count = 0;          /* number of stored terms   */
    static idx_t   last  = 1;          /* last integer examined   */

    while (count < (size_t)n) {
        /* generate next candidate prime */
        last++;
        if (last > 2 && (last & 1U) == 0) continue;   /* skip even numbers */

        if (!is_prime(last)) continue;                /* not prime */

        idx_t f1 = 2 * last + 9;                      /* 2p + 9   */
        idx_t f2 = 4 * last + 27;                     /* 4p + 27  */

        if (!is_prime(f1) || !is_prime(f2)) continue;

        /* store the qualifying prime, reallocating if needed */
        if (count == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = seq_realloc(cache, cap, new_cap);
            if (!tmp) abort();  /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }
        cache[count++] = (seq_t)last;
    }
    return cache[(size_t)n - 1];
}
