/*
 * A047222  Numbers that are congruent to {0, 2, 3} mod 5.
 * https://oeis.org/A047222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5*n-4)/3)
 * Range:     u8 n <= 154, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=2, a(3)=3, a(4)=5, a(n)=a(n-1)+a(n-3)-a(n-4) for n>4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;

    /* Allocate a minimal array to store computed values */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;  /* fallback for out‑of‑memory */

    /* Initialise the known values */
    arr[1] = (seq_t)0;
    arr[2] = (seq_t)2;
    arr[3] = (seq_t)3;
    arr[4] = (seq_t)5;

    /* Compute the sequence up to n using the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-3) - a(i-4) */
        arr[i] = arr[i - 1] + arr[i - 3] - arr[i - 4];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
