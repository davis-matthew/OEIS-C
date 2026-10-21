/*
 * A088674  Coefficients of the eigenfunction of a sequence transformation.
 * https://oeis.org/A088674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088674.
   Recurrence (derived from A(x^2) = (A(x/2)-1)/x - A(x/2)^2/2):
   a(0) = 1
   For n ≥ 1:
      if n even: a(n) = Σ_{i=0}^{n-1} a(i) * a(n-1-i)
      if n odd : a(n) = (a((n-1)/2) << n) + Σ_{i=0}^{n-1} a(i) * a(n-1-i)
   The shift uses the binary shift operator; it works for the built-in
   integer types and for arbitrary‑precision seq_t types.  The function
   memoises earlier values and works for any order of calls.  It
   requires O(n²) arithmetic operations and O(n) storage.  It is fast
   enough for n ≤ 10 000. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* stored terms a(0)...a(max)   */
    static idx_t size  = 0;         /* number of stored terms       */
    static idx_t cap   = 0;         /* allocated capacity           */

    /* initialise cache with a(0) = 1 */
    if (size == 0) {
        cap  = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        size = 1;
    }

    /* grow cache until it contains a(n) */
    if ((idx_t)n >= size) {
        /* enlarge capacity if necessary */
        if ((idx_t)n >= cap) {
            do { cap <<= 1; } while ((idx_t)n >= cap);
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        /* compute missing terms */
        for (idx_t k = size; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            idx_t h = k - 1;              /* upper index for the convolution */

            for (idx_t i = 0; i <= h; ++i) {
                sum += cache[i] * cache[h - i];
            }

            if ((k & 1) == 0) {           /* even n */
                cache[k] = sum;
            } else {                      /* odd n */
                idx_t mid = (k - 1) >> 1; /* (n-1)/2 */
                seq_t shift = cache[mid] << k; /* 2^n * a((n-1)/2) */
                cache[k] = sum + shift;
            }
        }
        size = n + 1;
    }

    return cache[n];
}
