/*
 * A227266  Number of n X 3 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 4 binary array having a sum of three or less, with
 *          rows and columns of the latter in lexicographically nondecreasing
 *          order.
 * https://oeis.org/A227266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 47, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227266 – Number of n×3 0‑1 arrays indicating 2×2 subblocks of
 * some (n+1)×4 binary array with a sum ≤ 3, with the rows and columns
 * of the latter in lexicographically nondecreasing order.
 *
 * Recurrence (order 10):
 *     a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) + 252*a(n-5)
 *            - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) - a(n-10)
 * for n > 13.
 *
 * Initial values (n = 1 … 13):
 *     4, 16, 49, 132, 341, 836, 1934, 4232, 8804, 17501,
 *     33392, 61393, 109141.
 *
 * The function evaluates the recurrence iteratively, keeping the
 * last ten values in a circular buffer.  It runs in O(n) time and
 * O(1) additional space.
 */

#include <stddef.h> /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Initial terms a(1)…a(13) */
    static const seq_t init[13] = {
        (seq_t)4   , (seq_t)16  , (seq_t)49  , (seq_t)132 ,
        (seq_t)341 , (seq_t)836 , (seq_t)1934 , (seq_t)4232 ,
        (seq_t)8804, (seq_t)17501, (seq_t)33392, (seq_t)61393,
        (seq_t)109141
    };

    if (n <= 13)
        return init[n - 1];

    /* Buffer holding a(i-10)…a(i-1) when i is the current index */
    seq_t last[10];
    for (idx_t i = 0; i < 10; ++i)
        last[i] = init[i + 3];          /* a(4)…a(13) for i = 14 */

    seq_t current = 0;

    for (idx_t i = 14; i <= n; ++i) {
        /* Compute positive and negative parts separately to avoid
           unsigned underflow while using the recurrence coefficients. */
        seq_t pos = 0;
        pos += last[9] * 10;   /* 10*a(i-1)  */
        pos += last[7] * 120;  /* 120*a(i-3) */
        pos += last[5] * 252;  /* 252*a(i-5) */
        pos += last[3] * 120;  /* 120*a(i-7) */
        pos += last[1] * 10;   /* 10*a(i-9)  */

        seq_t neg = 0;
        neg += last[8] * 45;   /* 45*a(i-2)  */
        neg += last[6] * 210;  /* 210*a(i-4) */
        neg += last[4] * 210;  /* 210*a(i-6) */
        neg += last[2] * 45;   /* 45*a(i-8)  */
        neg += last[0] * 1;    /* 1*a(i-10)  */

        current = pos - neg;

        /* Shift buffer left by one and insert the new value */
        for (idx_t k = 0; k < 9; ++k)
            last[k] = last[k + 1];
        last[9] = current;
    }

    return current;
}
