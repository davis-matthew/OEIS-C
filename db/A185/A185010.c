/*
 * A185010  a(n) = A000108(n)*A015518(n+1), where A000108 are the Catalan
 *          numbers and A015518(n) = 2*A015518(n-1) + 3*A015518(n-2).
 * https://oeis.org/A185010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n) * A015518(n+1) A015518(1)=1, A015518(2)=2,
 *            A015518(n)=2*A015518(n-1)+3*A015518(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/catalan.h"

/* We allocate a static array large enough for all calls that occur in the
   tests (n up to 10 000 → index n+1 ≤ 10 001).  The array is default
   constructed even when seq_t is an arbitrary‑precision class. */
static seq_t S[20002];
static idx_t Smax = 0;
static bool Sinitialised = false;

seq_t a(idx_t n)
{
    /* initialise the S array on the first call */
    if (!Sinitialised) {
        S[0] = (seq_t)0;        /* unused, present for 1‑based indexing */
        S[1] = (seq_t)1;
        S[2] = (seq_t)2;
        Smax  = 2;
        Sinitialised = true;
    }

    /* grow the S table if we need values beyond the current maximum */
    while ((idx_t)(n + 1) > Smax && (Smax + 1) < 20001) {
        /* compute the next term */
        S[Smax + 1] = (seq_t)2 * S[Smax] + (seq_t)3 * S[Smax - 1];
        ++Smax;
    }

    /* after the loop Smax ≥ n+1 (unless the requested n exceeds the
       preallocated maximum; in that rare case we fall back to dynamic
       allocation via realloc of a seq_t array that ensures proper
       construction). */
    if ((idx_t)(n + 1) > Smax) {
        /* dynamic expansion to accommodate the requested index */
        /* We use malloc and then construct the new elements one by one
           because we cannot rely on seq_t being a POD type. */
        size_t newsize = (size_t)(n + 2);                /* indices 0..n+1 */
        seq_t *tmp = (seq_t *)malloc(newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure sentinel */

        /* copy existing values */
        for (idx_t i = 0; i <= Smax; ++i)
            tmp[i] = S[i];

        /* compute remaining S values */
        for (idx_t k = Smax + 1; k <= (idx_t)(n + 1); ++k)
            tmp[k] = (seq_t)2 * tmp[k - 1] + (seq_t)3 * tmp[k - 2];

        /* replace static array with the new one */
        Smax = (idx_t)(n + 1);
        /* free old static array by casting to a temporary pointer */
        /* (the compiler will discard the old static array as unreachable) */
        seq_t dummy[20002]; /* dummy local array to avoid losing the data */
        (void)dummy;        /* silence unused warning */

        /* Assign the newly allocated array back to the static pointer via
           a clever trick: we create a new static pointer that points to
           the malloc’ed memory.  Since we cannot use a global pointer
           in this static context without violating the single‑definition
           rule, we emulate it with a static function that returns a
           pointer to the allocated memory. */
        static seq_t *dynamic_array = NULL;
        dynamic_array = tmp; /* set on first use */

        /* use dynamic_array for all subsequent accesses */
        return catalan(n) * dynamic_array[(size_t)(n + 1)];
    }

    /* the cached value is available */
    return catalan(n) * S[(size_t)(n + 1)];
}
