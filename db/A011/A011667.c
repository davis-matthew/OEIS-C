/*
 * A011667  A binary m-sequence: expansion of reciprocal of x^6+x^5+x^3+x^2+1.
 * https://oeis.org/A011667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) ^ a(n-3) ^ a(n-5) ^ a(n-6) (mod 2), * a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   alloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_arr.h"

static void free_arr(seq_t *p) {
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n) {
    static const seq_t init[6] = {0,0,0,0,0,1};

    if (n < 6) return init[n];

    size_t len = (size_t)(n + 1);
    seq_t *arr = alloc_arr(len);
    if (!arr) return (seq_t)0;          /* allocation failure – return 0 */

    for (idx_t i = 0; i < 6; ++i) arr[i] = init[i];

    for (idx_t i = 6; i <= n; ++i)
        arr[i] = arr[i-2] ^ arr[i-3] ^ arr[i-5] ^ arr[i-6];

    seq_t result = arr[n];
    free_arr(arr);
    return result;
}
