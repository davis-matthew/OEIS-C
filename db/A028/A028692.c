/*
 * A028692  23-factorial numbers.
 * https://oeis.org/A028692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 23-factorial numbers
   Formula:  a(n) = ∏_{k=1..n} (23^k - 1)
   a(0) = 1
   a(1) = 22
   a(2) = 11616
   ...
   The implementation memoizes computed values so that repeated
   calls for arbitrary indices are handled efficiently.  It uses only
   standard C library calls; no external helper functions are required.
*/

#include <stdlib.h>

/* Compute a(n) = Product_{k=1..n} (23^k - 1).  All arithmetic is
   performed in the type seq_t, which is provided by the build system. */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;   /* array of a(i) for i <= max_c   */
    static size_t cap   = 0;      /* allocated size of cache         */
    static idx_t  max_c = 0;      /* largest index currently stored  */
    static seq_t acc    = (seq_t)1;  /* value of a(max_c)              */
    static seq_t pow23  = (seq_t)23; /* current value 23^(max_c + 1)   */

    /* first call: allocate cache and set a(0) */
    if (cache == NULL) {
        cap   = 8;                        /* initial capacity               */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();              /* allocation failure             */
        cache[0] = (seq_t)1;              /* a(0) = 1                       */
        /* acc and pow23 are already set to initial values */
    }

    /* already computed? */
    if (n <= max_c)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* reallocation failure          */
        cache = tmp;
        cap   = new_cap;
    }

    /* fill cache from max_c+1 up to n */
    for (idx_t i = max_c + 1; i <= n; ++i) {
        seq_t term = pow23 - (seq_t)1;   /* (23^i - 1) */
        acc *= term;                    /* accumulate product            */
        cache[i] = acc;                 /* store a(i)                    */
        pow23 *= (seq_t)23;              /* advance to 23^(i+1)           */
    }

    max_c = n;
    return cache[n];
}
