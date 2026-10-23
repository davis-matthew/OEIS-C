/*
 * A116742  Number of permutations of length n which avoid the patterns 1342,
 *          2341, 4132; or avoid the patterns 2431, 3124, 4231.
 * https://oeis.org/A116742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 23*a(n-2) + 28*a(n-3) - 13*a(n-4) + 2*a(n-5)
 *            Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence comes from the rational generating function
   A(x) = -(x(2x⁴-9x³+13x²-6x+1))/((2x-1)(x²-3x+1)²). */

#include <stddef.h>
#include <stdlib.h>

/* The function signature required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is defined for n ≥ 1, but avoid UB */
        return 0;

    /* allocate an array large enough to hold terms 0..n */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr)
        return 0; /* allocation failure – unlikely in tests */

    arr[0] = 0;
    if (n >= 1) arr[1] = 1;
    if (n >= 2) arr[2] = 2;
    if (n >= 3) arr[3] = 6;
    if (n >= 4) arr[4] = 21;
    if (n >= 5) arr[5] = 75;

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute positive and negative parts separately to avoid
           underflow on unsigned types.  The final result is always
           non‑negative. */
        seq_t pos = (seq_t)8 * arr[i - 1]
                  + (seq_t)28 * arr[i - 3]
                  + (seq_t)2  * arr[i - 5];
        seq_t neg = (seq_t)23 * arr[i - 2]
                  + (seq_t)13 * arr[i - 4];
        arr[i] = pos - neg;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
