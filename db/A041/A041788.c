/*
 * A041788  Numerators of continued fraction convergents to sqrt(415).
 * https://oeis.org/A041788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(415).
   Recurrence: a(0)=20, a(1)=41, and for n>=2
      a(n) = c[(n-1) mod 16] * a(n-1) + a(n-2)
   where c = {2,1,2,4,6,1,1,3,1,1,6,4,2,1,2,40}. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cached numerators.  arr[i] stores a(i) for i < next. */
    static seq_t *arr  = NULL;
    static idx_t next  = 0; /* number of computed entries (size of valid prefix) */
    static idx_t cap   = 0; /* current allocation size */

    /* Periodic block of continued‑fraction coefficients. */
    static const uint8_t period[16] = {
        2, 1, 2, 4, 6, 1, 1, 3,
        1, 1, 6, 4, 2, 1, 2, 40
    };

    /* Initialise storage with a(0). */
    if (arr == NULL) {
        cap  = 1;
        arr  = (seq_t*)malloc(cap * sizeof(seq_t));
        arr[0] = (seq_t)20;   /* a(0) */
        next = 1;
    }

    /* Return immediately if only a(0) is needed. */
    if (n == 0) {
        return arr[0];
    }

    /* Ensure capacity for indices up to n. */
    if ((size_t)n >= (size_t)cap) {
        size_t newcap = (size_t)cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(arr, newcap * sizeof(seq_t));
        arr  = tmp;
        cap  = (idx_t)newcap;
    }

    /* Compute a(1) only if not already present. */
    if (next <= 1) {          /* only a(0) stored so far */
        arr[1] = (seq_t)41;   /* a(1) */
        next   = 2;
    }

    /* Compute successive terms up to n. */
    for (idx_t i = next; i <= n; ++i) {
        uint8_t coeff = period[(i - 1) % 16];
        arr[i] = (seq_t)coeff * arr[i - 1] + arr[i - 2];
    }
    next = n + 1;              /* all indices < next are valid now */

    return arr[n];
}
