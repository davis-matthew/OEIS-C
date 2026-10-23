/*
 * A043240  Numbers k such that 3 and 4 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th decimal integer k such that the decimal
 *            representation of k contains the substrings "34" or "43", while
 *            the preceding integer k-1 does not contain those substrings
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_34_or_43
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static array to memoise all
   found terms.  It is efficient enough for n up to at least
   10000 and works for any seq_t that supports the
   integer arithmetic operators used below. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_adjacent_34_or_43.h"

/* Static table to store the sequence values.  The size is
   chosen large enough for the tested range (>= 10000).  The
   code will keep generating terms until it has produced
   at least n elements. */
#define CACHE_MAX 200000
static seq_t cache[ CACHE_MAX ];
static size_t cache_size = 0;
static seq_t current = (seq_t)33;   /* number last tested */

/* Public function: returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n < 1, safe return */

    /* Generate terms until we have at least n stored. */
    while (cache_size < (size_t)n) {
        current = current + (seq_t)1;          /* examine next integer */

        if ( has_adjacent_34_or_43(current) &&
             !has_adjacent_34_or_43(current - (seq_t)1) ) {

            if (cache_size >= CACHE_MAX) {
                /* In the unlikely event the buffer is full, stop
                   and return the last found value.  For the expected
                   input ranges this condition never occurs. */
                break;
            }
            cache[ cache_size ] = current;
            cache_size++;
        }
    }

    /* n is guaranteed to be <= cache_size after the loop. */
    return cache[ (size_t)(n-1) ];
}
