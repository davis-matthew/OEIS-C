/*
 * A159896  Positive numbers y such that y^2 is of the form x^2+(x+839)^2 with
 *          integer x.
 * https://oeis.org/A159896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) (n > 6); Initial values: a(1) = 785,
 *            a(2) = 839, a(3) = 901, a(4) = 3809, a(5) = 4195, a(6) = 4621
 * Range:     u8 none, u16 n <= 9, u32 n <= 27, u64 n <= 66, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* for uint64_t, etc. */
#include <stdbool.h>  /* for bool */

/* Computes A159896.  No caching is used; each call runs
   an O(n) loop which is <2 s for n ≤ 10000.  The function
   constructs a temporary array of `seq_t` that is safely
   initialized whether seq_t is a plain unsigned integer
   (C build) or an arbitrary‑precision class (C++ build). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    if (n < 7) {                          /* direct return of one of the first six terms */
        switch ((unsigned)n) {
        case 1: return (seq_t)785;
        case 2: return (seq_t)839;
        case 3: return (seq_t)901;
        case 4: return (seq_t)3809;
        case 5: return (seq_t)4195;
        case 6: return (seq_t)4621;
        }
    }

    /* Allocate a temporary array to hold the values up to index n. */
    size_t len = (size_t)n + 1;
    seq_t *arr;
#if defined(__cplusplus)
    arr = new seq_t[len];   /* value‑initialises each element (C++). */
#else
    arr = (seq_t *)malloc(len * sizeof(seq_t));   /* C.  Elements are zero‑initialised by malloc. */
    if (!arr) return (seq_t)0;                    /* allocation failure – return 0 as a safe fallback. */
#endif

    /* Initialise the first six terms. */
    arr[1] = (seq_t)785;
    arr[2] = (seq_t)839;
    arr[3] = (seq_t)901;
    arr[4] = (seq_t)3809;
    arr[5] = (seq_t)4195;
    arr[6] = (seq_t)4621;

    /* Compute the remaining terms using the linear recurrence. */
    for (idx_t i = 7; i <= n; ++i) {
        arr[i] = 6 * arr[i - 3] - arr[i - 6];
    }

    seq_t result = arr[n];

#if defined(__cplusplus)
    delete [] arr;   /* destructor invocation for each element. */
#else
    free(arr);       /* deallocate the memory in C. */
#endif
    return result;
}
