/*
 * A207689  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+3*a(n-3)+a(n-4)-a(n-5)-a(n-6) for n>8
 *            Initial values: a(1)=9, a(2)=81, a(3)=100, a(4)=256, a(5)=576,
 *            a(6)=1156, a(7)=2500, a(8)=5476
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 25, u64 n <= 54, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, free, size_t */

seq_t a(idx_t n)
{
    /* a(n) is defined only for n>=1; caller must ensure n>=1. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure the caller can use values that fit in seq_t. */
    static const seq_t init[9] = {
        (seq_t)0,      /* placeholder for index 0 */
        (seq_t)9,      /* a(1) */
        (seq_t)81,     /* a(2) */
        (seq_t)100,    /* a(3) */
        (seq_t)256,    /* a(4) */
        (seq_t)576,    /* a(5) */
        (seq_t)1156,   /* a(6) */
        (seq_t)2500,   /* a(7) */
        (seq_t)5476    /* a(8) */
    };

    /* Allocate space for the sequence up to n. */
    size_t len = (size_t)n + 1;
    seq_t *seq;
#ifdef __cplusplus
    seq = new seq_t[len];
#else
    seq = (seq_t*)malloc(len * sizeof(seq_t));
#endif
    if (!seq) return (seq_t)0;          /* allocation failed */

    /* Load known initial values. */
    size_t i;
    for (i = 1; i <= 8 && i <= n; ++i)
        seq[i] = init[i];

    /* Compute remaining terms using the recurrence. */
    for (; i <= n; ++i)
        seq[i] = seq[i-1] + seq[i-2] + (seq_t)3 * seq[i-3] + seq[i-4]
                 - seq[i-5] - seq[i-6];

    seq_t answer = seq[n];

    /* Clean up. */
#ifdef __cplusplus
    delete[] seq;
#else
    free(seq);
#endif

    return answer;
}
