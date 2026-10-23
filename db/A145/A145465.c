/*
 * A145465  a(0) = 2, a(1) = 2, and for n > 1, a(n) = a(n-1) + a((a(n-1) - 1)
 *          mod n).
 * https://oeis.org/A145465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A145465
   a(0) = 2, a(1) = 2
   a(n) = a(n-1) + a((a(n-1)-1) mod n)  for n > 1
*/

#include <stddef.h>
#include <stdlib.h>

/* Compute A145465 for any n ≥ 0.
   The function works for all supported seq_t / idx_t types
   and uses the same implementation both in C and C++.
 */
seq_t a(idx_t n) {
    /* Handle the two base cases. */
    if (n == 0 || n == 1) {
        return (seq_t)2;
    }

    /* Allocate storage for all terms up to n. */
    size_t sz = (size_t)n + 1;              /* safe: n+1 fits in size_t */
    seq_t *arr;
#if defined(__cplusplus)
    arr = new seq_t[sz];
#else
    arr = (seq_t*)malloc(sz * sizeof(seq_t));
#endif

    /* Initialize the first two terms. */
    arr[0] = (seq_t)2;
    arr[1] = (seq_t)2;

    /* Iterate, filling the array. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t prev = arr[i-1];
        idx_t k = (idx_t)((prev - (seq_t)1) % (seq_t)i);
        arr[i] = prev + arr[k];
    }

    seq_t result = arr[n];

    /* Free the allocated memory. */
#if defined(__cplusplus)
    delete[] arr;
#else
    free(arr);
#endif

    return result;
}
