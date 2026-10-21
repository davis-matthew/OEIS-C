/*
 * A074056  a(n) = a(n-1)^3/a(n-2)^2 + a(n-1)*a(n-2) with a(1) = a(2) = 1.
 * https://oeis.org/A074056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074056: a(n) = a(n-1)^3/a(n-2)^2 + a(n-1)*a(n-2) with a(1)=a(2)=1 */

#include <stddef.h>
#include <stdlib.h>

/* Returns the nth term of A074056 (1‑based index).  The function
   memoises already computed values so calls may arrive in any order. */
seq_t a(idx_t n)
{
    /* Return a sentinel value for indices below the offset. */
    if (n < 1)
        return (seq_t)0;

    /* Lazy static state ---------------------------------------------------- */
    static seq_t *arr = NULL;          /* 1‑based array, arr[0] unused */
    static idx_t cur_max = 0;          /* highest index computed so far    */
    static size_t arr_alloc = 0;       /* allocated array size (>= n+1)    */
    static int initialized = 0;        /* flag for one‑time initialisation */

    /* Initialise on first call. ------------------------------------------------ */
    if (!initialized) {
        /* Start with a modest capacity; it will grow on demand. */
        arr_alloc = 8;
        arr = (seq_t *)realloc(NULL, arr_alloc * sizeof(seq_t));
        if (!arr) return (seq_t)0; /* allocation failure: give up */

        /* Zero the whole array to keep the unused entry at index 0 clear. */
        for (size_t i = 0; i < arr_alloc; ++i) {
            arr[i] = (seq_t)0;
        }

        /* Base cases: a(1) = a(2) = 1. */
        arr[1] = (seq_t)1;
        arr[2] = (seq_t)1;
        cur_max = 2;

        initialized = 1;
    }

    /* Fast return for known indices. ---------------------------------------- */
    if (n <= cur_max)
        return arr[n];

    /* Ensure the underlying array is large enough. ----------------------------- */
    if ((size_t)(n + 1) > arr_alloc) {
        size_t new_alloc = arr_alloc;
        while ((size_t)(n + 1) > new_alloc)
            new_alloc <<= 1; /* double until sufficient */

        seq_t *tmp = (seq_t *)realloc(arr, new_alloc * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */

        /* Initialise the newly added region to zero. */
        for (size_t i = arr_alloc; i < new_alloc; ++i)
            tmp[i] = (seq_t)0;

        arr = tmp;
        arr_alloc = new_alloc;
    }

    /* Compute all missing terms up to n in order. ---------------------------- */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t a_prev = arr[i - 1];
        seq_t a_prevprev = arr[i - 2];

        /* Compute a_prev^3.  The intermediate products use the type's
           arithmetic operators which are safe for both fixed‑size and
           arbitrary‑precision seq_t. */
        seq_t cube = a_prev * a_prev;
        cube *= a_prev; /* a_prev^3 */

        seq_t square = a_prevprev * a_prevprev; /* a_prevprev^2 */

        /* Division is exact by definition of the sequence. */
        seq_t div = cube / square;

        /* Final recurrence term. */
        arr[i] = div + a_prev * a_prevprev;
    }

    cur_max = n;
    return arr[n];
}
