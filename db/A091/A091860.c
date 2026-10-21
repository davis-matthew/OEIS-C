/*
 * A091860  a(1)=1, a(n)=sum(i=1,n-1,b(i)) where b(i)=0 if a(i) and a(n-i) are
 *          both even, b(i)=1 otherwise.
 * https://oeis.org/A091860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, for n ≥ 2: if n is even: a(n) = a(n/2) + 2
 *            if n is odd (n = 2m+1): base = a(2m); if m is a power of 2: a(n)
 *            = base + 1 else if m has exactly two 1‑bits: a(n) = base + 2
 *            else: a(n) = base
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)1;

    idx_t size = n + 1;
    seq_t *arr;
#if defined(__cplusplus)
    arr = new seq_t[size];
#else
    arr = (seq_t*)malloc(size * sizeof(seq_t));
#endif
    if (!arr) abort();

    arr[1] = (seq_t)1;
    arr[2] = (seq_t)1;

    for (idx_t i = 3; i <= n; ++i) {
        if ((i & 1ULL) == 0) {                     /* even i */
            arr[i] = arr[i / 2] + (seq_t)2;
        } else {                                   /* odd i */
            idx_t m = i >> 1;                      /* m = (i-1)/2 */
            seq_t base = arr[i - 1];               /* a(2m) */

            unsigned int pop = __builtin_popcountll(m);
            unsigned int is_pow2 = (m & (m - 1ULL)) == 0ULL;

            seq_t inc = (seq_t)0;
            if (is_pow2)
                inc = (seq_t)1;
            else if (pop == 2u)
                inc = (seq_t)2;

            arr[i] = base + inc;
        }
    }

    seq_t result = arr[n];
#if defined(__cplusplus)
    delete[] arr;
#else
    free(arr);
#endif
    return result;
}
