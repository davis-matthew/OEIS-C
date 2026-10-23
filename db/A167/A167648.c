/*
 * A167648  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 2 a(n-1) - a(n-2)
 *            + 2 a(n-3) - a(n-4) + 2 a(n-5) - a(n-6) * + 2 a(n-7) - a(n-8) +
 *            2 a(n-9) - a(n-10) + 2 a(n-11) - a(n-12) * + 2 a(n-13) - a(n-14)
 *            for n >= 15, * with initial values a(0)=1, a(1)=3, a(2)=6, …,
 *            a(14)=24576
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free */

/* Initial 15 terms: a(0) to a(14). */
static const seq_t init[15] = {
    (seq_t)1,  (seq_t)3,  (seq_t)6,   (seq_t)12,  (seq_t)24,
    (seq_t)48, (seq_t)96, (seq_t)192, (seq_t)384, (seq_t)768,
    (seq_t)1536,(seq_t)3072,(seq_t)6144,(seq_t)12288,(seq_t)24576
};

seq_t a(idx_t n)
{
    /* For the first 15 terms return the stored constants. */
    if (n < 15) {
        return init[(size_t)n];
    }

    /* Allocate a temporary array large enough to hold the whole sequence up
     * to the requested index.  We avoid global or static storage so the
     * function can be called in any order.  The allocation uses the
     * appropriate dynamic memory allocator for C or C++.
     */
    size_t len = (size_t)n + 1;
#ifdef __cplusplus
    seq_t* arr = new seq_t[len];
#else
    seq_t* arr = (seq_t*)malloc(len * sizeof(seq_t));
#endif
    if (!arr) {
        /* In the unlikely event of allocation failure, return 0.  The
         * sequence never contains 0 for n ≥ 0, so this is an error.
         */
        return (seq_t)0;
    }

    /* Initialise the array with the 15 known terms. */
    for (size_t i = 0; i < 15; ++i) {
        arr[i] = init[i];
    }

    /* Apply the recurrence for all subsequent indices. */
    for (size_t i = 15; i <= (size_t)n; ++i) {
        seq_t val = 0;
        /* Explicitly unrolled recurrence for efficiency. */
        val = 2 * arr[i - 1];
        val -= arr[i - 2];
        val += 2 * arr[i - 3];
        val -= arr[i - 4];
        val += 2 * arr[i - 5];
        val -= arr[i - 6];
        val += 2 * arr[i - 7];
        val -= arr[i - 8];
        val += 2 * arr[i - 9];
        val -= arr[i -10];
        val += 2 * arr[i -11];
        val -= arr[i -12];
        val += 2 * arr[i -13];
        val -= arr[i -14];
        arr[i] = val;
    }

    /* Retrieve the requested term. */
    seq_t result = arr[n];

    /* Clean up. */
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif

    return result;
}
