/*
 * A160055  Positive numbers y such that y^2 is of the form x^2+(x+89)^2 with
 *          integer x.
 * https://oeis.org/A160055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=65, a(2)=89, a(3)=149, a(4)=241, a(5)=445, a(6)=829; a(n) =
 *            6*a(n-3) - a(n-6) for n > 6. (OEIS A160055)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 32, u64 n <= 70, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holds computed terms.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t last = 0;   /* highest index that has been computed */

    /* Guard against an illegal request, although the specification
       guarantees n >= 1.  Returning 0 for n==0 is harmless. */
    if (n == 0) return (seq_t)0;

    /* Ensure the cache is large enough for index n.  reallocate if needed. */
    if (n > last) {
        size_t new_size = (size_t)(n + 1);          /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();                  /* out of memory */
        cache = tmp;

        /* Compute all newly needed terms up to n. */
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 1) cache[i] = 65;
            else if (i == 2) cache[i] = 89;
            else if (i == 3) cache[i] = 149;
            else if (i == 4) cache[i] = 241;
            else if (i == 5) cache[i] = 445;
            else if (i == 6) cache[i] = 829;
            else /* i > 6 */ cache[i] = 6 * cache[i-3] - cache[i-6];
        }
        last = n;
    }

    return cache[n];
}
