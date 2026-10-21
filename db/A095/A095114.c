/*
 * A095114  a(1)=1. a(n) = a(n-1) + (number of elements of {a(1),...,a(n-1)}
 *          that are <= n-1).
 * https://oeis.org/A095114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + (# of elements of {a(1),...,a(n-1)} that are <= n-1), a(1)=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence defined for n >= 1 */

    idx_t size = n + 1;                    /* 1‑based array, element 0 unused */
    seq_t *arr = NULL;

#if defined(__cplusplus)
    /* C++ build – use new, which calls constructors for arbitrary‑precision types */
    arr = new seq_t[size];
#else
    /* C build – malloc is sufficient for POD seq_t types */
    arr = (seq_t *)malloc(sizeof(seq_t) * size);
#endif

    if (!arr) abort();                     /* allocation failed */

    arr[0] = (seq_t)0;                     /* placeholder */
    arr[1] = (seq_t)1;                     /* a(1) */

    idx_t j = 0;                           /* number of a(k) <= current - 1 */

    for (idx_t i = 2; i <= n; ++i) {
        /* advance j while a(j+1) <= i-1 */
        while (j + 1 <= i - 1 && arr[j + 1] <= (seq_t)(i - 1)) {
            ++j;
        }
        arr[i] = arr[i - 1] + (seq_t)j;
    }

    seq_t result = arr[n];

#if defined(__cplusplus)
    delete [] arr;
#else
    free(arr);
#endif

    return result;
}
