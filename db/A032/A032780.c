/*
 * A032780  a(n) = n(n+1)(n+2)...(n+8) / (n+(n+1)+(n+2)+...+(n+8)).
 * https://oeis.org/A032780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) (for n >
 *            8). Initial values: a(0)=0, a(1)=8064, a(2)=67200, a(3)=316800,
 *            a(4)=1108800, a(5)=3203200, a(6)=8072064, a(7)=18345600,
 *            a(8)=38438400. This recurrence yields exactly the OEIS se...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 17, u64 n <= 332, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first nine terms directly. */
    static const seq_t init[9] = {
        (seq_t)0,
        (seq_t)8064,
        (seq_t)67200,
        (seq_t)316800,
        (seq_t)1108800,
        (seq_t)3203200,
        (seq_t)8072064,
        (seq_t)18345600,
        (seq_t)38438400
    };

    if (n < 9u)
        return init[(size_t)n];

    /* Allocate an array of length n+1 (size_t is safe for the allocation
       size since n is non-negative). */
    size_t len = (size_t)n + 1u;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (arr == NULL) { /* Fallback for OOM: return 0 */
        return (seq_t)0;
    }

    /* Initialise the first nine elements. */
    for (size_t i = 0u; i < 9u; ++i)
        arr[i] = init[i];

    /* Compute subsequent terms using the 9‑term linear recurrence. */
    for (size_t i = 9u; i < len; ++i) {
        arr[i] = (seq_t)(
            (seq_t)9u * arr[i-1] -
            (seq_t)36u * arr[i-2] +
            (seq_t)84u * arr[i-3] -
            (seq_t)126u * arr[i-4] +
            (seq_t)126u * arr[i-5] -
            (seq_t)84u * arr[i-6] +
            (seq_t)36u * arr[i-7] -
            (seq_t)9u  * arr[i-8] +
            arr[i-9]
        );
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
