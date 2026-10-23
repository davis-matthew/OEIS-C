/*
 * A042398  Numerators of continued fraction convergents to sqrt(726).
 * https://oeis.org/A042398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 970*a(n-4) - a(n-8) for n >= 8 Initial values: a(0)=26,
 *            a(1)=27, a(2)=458, a(3)=485, a(4)=25678, a(5)=26163,
 *            a(6)=444286, a(7)=470449
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* constant initial terms */
    static const seq_t base[8] = {
        (seq_t)26,   (seq_t)27,   (seq_t)458, (seq_t)485,
        (seq_t)25678,(seq_t)26163,(seq_t)444286,(seq_t)470449
    };

    /* fast return for the first 8 indices */
    if (n < 8) {
        return base[n];
    }

    /* For moderate n (<= 10000) use a static buffer on the stack.
       For larger n, allocate a dynamic array that is properly
       constructed depending on the language (new[] in C++ or malloc in C). */
    if (n <= 10000) {
        static seq_t buf[10001];
        idx_t i;

        /* initialise the first 8 values */
        for (i = 0; i < 8; ++i) {
            buf[i] = base[i];
        }

        /* compute the sequence up to n */
        for (i = 8; i <= n; ++i) {
            buf[i] = (seq_t)970 * buf[i - 4] - buf[i - 8];
        }

        return buf[n];
    } else {
        /* large n: dynamic allocation */
        seq_t *arr;
#ifdef __cplusplus
        arr = new seq_t[n + 1];
#else
        arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
        if (!arr) {
            /* allocation failed – return zero as a safe fallback */
            return 0;
        }

        /* initialise the first 8 values */
        idx_t i;
        for (i = 0; i < 8; ++i) {
            arr[i] = base[i];
        }

        /* compute the sequence up to n */
        for (i = 8; i <= n; ++i) {
            arr[i] = (seq_t)970 * arr[i - 4] - arr[i - 8];
        }

        seq_t result = arr[n];

        /* free the allocation */
#ifdef __cplusplus
        delete [] arr;
#else
        free(arr);
#endif
        return result;
    }
}
