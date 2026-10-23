/*
 * A168629  Numbers n such that n,n+1 and sum of this two numbers have at
 *          least 3 distinct prime factors.
 * https://oeis.org/A168629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168629: term is the n‑th integer n such that
    n, n+1 and 2*n+1 each have at least 3 distinct prime factors
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

/* external linkage required by the build system */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *seq   = NULL;    /* array of terms */
    static size_t   cap   = 0;     /* allocated capacity */
    static size_t   sz    = 0;     /* number of stored terms */
    static idx_t    cur   = 1;     /* current integer to test */

    /* allocate or grow storage when needed */
    if (sz == 0) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) abort();
    }

    /* collect terms until we have at least n of them */
    while (sz < n) {
        uint64_t x = (uint64_t)cur;
        /* test the three numbers for at least 3 distinct prime factors */
        if (   count_distinct_primes(x)        >= 3
            && count_distinct_primes(x + 1)   >= 3
            && count_distinct_primes(2 * x + 1) >= 3) {
            /* increase capacity if necessary before storing */
            if (sz == cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq = tmp;
                cap = new_cap;
            }
            seq[sz++] = (seq_t)x;
        }
        ++cur;
    }

    return seq[n - 1];   /* return the n‑th term (1‑based index) */
}
