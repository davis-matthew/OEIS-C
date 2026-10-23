/*
 * A174821  Numbers n such that n^4 - n^2 - 1 is prime.
 * https://oeis.org/A174821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Function for OEIS A174821: numbers n such that n^4 - n^2 - 1 is prime
 * Formula: a(n) is the n‑th integer >1 for which n^4 - n^2 - 1 is prime.
 *
 * The implementation keeps a static cache, extending it as needed, so
 * that repeated calls are fast and independent of order.  It uses
 * libutils helpers so it works for all supported seq_t types
 * (builtin integers down to 8 bits, or arbitrary‑precision classes).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Return the n-th element of sequence A174821 (offset 1). */
seq_t a(idx_t n)
{
    /* Static cache data */
    static seq_t *cache = NULL;      /* array of found terms    */
    static idx_t cache_cap = 0;      /* capacity of cache      */
    static idx_t found = 0;          /* number of terms stored */
    static idx_t last_checked = 1;   /* last candidate checked */

    /* Grow cache if necessary */
    if (cache_cap < n) {
        size_t new_cap = (cache_cap == 0) ? n : (cache_cap * 2);
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) return (seq_t)0;   /* memory error – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Search for additional terms until we have at least n of them */
    while (found < n) {
        ++last_checked;                   /* next candidate integer */
        seq_t m   = (seq_t)last_checked;  /* cast to seq_t */

        /* Compute m^4 - m^2 - 1 */
        seq_t sq   = m * m;           /* m^2          */
        seq_t term = sq * sq - sq - (seq_t)1; /* m^4 - m^2 - 1 */

        /* Test for primality of term */
        if (is_prime_big(term)) {
            /* Store the candidate as the next sequence element */
            cache[found++] = m;
        }
    }

    /* Return the requested element (offset 1) */
    return cache[n - 1];
}
