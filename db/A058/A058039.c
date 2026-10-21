/*
 * A058039  a(n) = a(n-1) + 2*a(floor(n/2)) if n > 0, otherwise 1.
 * https://oeis.org/A058039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(floor(n/2)), a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 286, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple dynamic array that is allocated anew on every call.
   It works in C11 (via malloc/free) and in C++17 (via new[]/delete[]).
   The array holds seq_t values; for scalar types this is straightforward,
   and for arbitrary-precision types the C++ branch properly constructs the
   objects. */

#include <stdlib.h>   /* malloc, free, NULL */

seq_t a(idx_t n)
{
    /* Handle the base case directly. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array of (n+1) seq_t elements. */
#if defined(__cplusplus)
    /* C++: use operator new[] to default‑construct the elements. */
    seq_t *arr = new seq_t[n + 1];
    if (!arr) return (seq_t)0;   /* allocation failure – return zero */
#else
    /* C: use malloc – seq_t is expected to be a plain integer type. */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;   /* allocation failure – return zero */
#endif

    /* Initialize the first element. */
    arr[0] = (seq_t)1;

    /* Compute following terms iteratively. */
    for (idx_t i = 1; i <= n; ++i) {
        arr[i] = arr[i - 1] + (seq_t)2 * arr[i / 2];
    }

    seq_t result = arr[n];

    /* Clean up. */
#if defined(__cplusplus)
    delete[] arr;
#else
    free(arr);
#endif

    return result;
}
