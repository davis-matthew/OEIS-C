/*
 * A172462  Numbers k such that 2k-3, 2k-1, 2k+1 and 2k+3 are composite.
 * https://oeis.org/A172462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 2k-3, 2k-1, 2k+1 and 2k+3 are composite.
   The function returns the nth such number (offset 1).  It searches
   candidates incrementally, memoises the results, and relies on
   isPrimeGeneric to test primality of the 4 numbers. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *list   = NULL;   /* array of found k's          */
    static size_t  size  = 0;      /* number of elements stored   */
    static size_t  cap   = 0;      /* current capacity of list    */
    static seq_t   next  = 2;      /* next candidate k to test    */

    /* guard against out-of-range requests */
    if (n < 1) return (seq_t)0;

    /* iterate until we have at least n elements */
    while (size < n) {
        seq_t k = next++;

        /* compute the four numbers to test */
        seq_t a1 = (seq_t)2 * k - (seq_t)3;
        seq_t a2 = (seq_t)2 * k - (seq_t)1;
        seq_t a3 = (seq_t)2 * k + (seq_t)1;
        seq_t a4 = (seq_t)2 * k + (seq_t)3;

        /* check that each is composite: >1 and not prime */
        bool ok = (a1 > (seq_t)1 && !isPrimeGeneric(a1)) &&
                  (a2 > (seq_t)1 && !isPrimeGeneric(a2)) &&
                  (a3 > (seq_t)1 && !isPrimeGeneric(a3)) &&
                  (a4 > (seq_t)1 && !isPrimeGeneric(a4));

        if (ok) {
            /* grow the array if needed */
            if (size == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp) abort();               /* unrecoverable failure */
                list = tmp;
                cap  = new_cap;
            }
            list[size++] = k;
        }
    }

    return list[n - 1];
}
