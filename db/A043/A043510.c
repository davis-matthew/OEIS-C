/*
 * A043510  Numbers having two 5's in base 10.
 * https://oeis.org/A043510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer that contains exactly two digits '5' in
 *            base 10
 * Range:     u8 n <= 3, u16 n <= 5603, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count5_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes terms on demand and caches the results in a
   fixed-size static array.  It works for both built‑in integral types and for
   arbitrary‑precision integer classes. */

#include <stddef.h>    /* for size_t */
#include <stdbool.h>   /* for bool */
#include "../utils/count5_digits.h"

/* Cache large enough for the typical usage (≈10 000 terms). */
#define MAX_TERMS 20000
static seq_t cache[MAX_TERMS];
static size_t cache_len = 0;       /* number of terms currently stored */
static seq_t last_checked = 0;     /* largest integer examined so far */

/* Return the nth term of A043510 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset 1: a(0) is undefined in OEIS, return 0 */

    /* Generate terms until we have at least n of them.  If n exceeds the
       maximum preallocated size the function will continue to generate
       terms but will not store them in the cache. */
    while (cache_len < n) {
        /* Move to the next integer.  The type seq_t supports the increment
           operator. */
        ++last_checked;

        if (count5_digits(last_checked) == (idx_t)2) {
            if (cache_len < MAX_TERMS) {
                cache[cache_len++] = last_checked;
            }
        }
    }

    /* cached terms are always available up to cache_len. */
    return cache[(size_t)n - 1];
}
