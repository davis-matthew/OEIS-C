/*
 * A079398  a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(n) = a(n-3) + a(n-4) for n > 3.
 * https://oeis.org/A079398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 1, a(n) = a(n-3) + a(n-4)
 *            (n > 3)
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 113, u64 n <= 225,
 *            u128 n <= 447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is completely recomputed for each call.  The
   recursion is evaluated iteratively, so the algorithm runs in
   O(n) time and uses O(n) additional memory, which is well below
   the 2‑second limit for n ≤ 10 000.  It works for both
   64‑bit integer builds and arbitrary‑precision builds (the
   latter compile as C++).  No external helper functions are
   required. */

#include <stdlib.h>    /* malloc, free, abort */

#ifdef __cplusplus
# include <new>        /* no-throw operator delete for arrays */
#endif

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2 || n == 3) return (seq_t)1;

    /* Allocate an array of size n+1 that holds all intermediate
       values.  For a C target (seq_t is an unsigned integer type)
       the allocation is trivial.  For a C++ target the custom
       type is constructed correctly by operator new[]. */
#ifdef __cplusplus
    seq_t *arr = ::new seq_t[n + 1];   /* value‐initialised */
#else
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) abort();
#endif

    arr[0] = (seq_t)0;
    arr[1] = (seq_t)1;
    arr[2] = (seq_t)1;
    arr[3] = (seq_t)1;

    for (idx_t i = 4; i <= n; ++i)
    {
        arr[i] = arr[i - 3] + arr[i - 4];
    }

    seq_t result = arr[n];

#ifdef __cplusplus
    ::delete[] arr;
#else
    free(arr);
#endif
    return result;
}
