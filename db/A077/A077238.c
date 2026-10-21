/*
 * A077238  Combined Diophantine Chebyshev sequences A077236 and A077235.
 * https://oeis.org/A077238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 4, a(1) = 5, a(2) = 11, a(3) = 16 a(n) = 4*a(n-2) -
 *            a(n-4) for n ≥ 4 The sequence also satisfies a(2*k)=A077236(k)
 *            and a(2*k+1)=A077235(k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 65, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* small constant terms */
    static const seq_t base4[4] = { (seq_t)4, (seq_t)5,
                                   (seq_t)11, (seq_t)16 };

    if (n < 4)                     /* return the first four terms directly */
        return base4[(size_t)n];

    size_t len = (size_t)n + 1;    /* array length */
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr)                      /* allocation failed */
        return (seq_t)0;

    /* initialise first four values */
    arr[0] = base4[0];
    arr[1] = base4[1];
    arr[2] = base4[2];
    arr[3] = base4[3];

    /* recurrence: a(n) = 4*a(n-2) - a(n-4) */
    for (size_t i = 4; i < len; ++i)
        arr[i] = (seq_t)4 * arr[i - 2] - arr[i - 4];

    seq_t result = arr[n];
    free(arr);
    return result;
}
