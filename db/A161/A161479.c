/*
 * A161479  Positive numbers y such that y^2 is of the form x^2+(x+113)^2 with
 *          integer x.
 * https://oeis.org/A161479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A161479
   Formula: a(n) = 6*a(n-3) - a(n-6)  for n > 6
   Initial values: a(1)=85, a(2)=113, a(3)=173, a(4)=337, a(5)=565, a(6)=953
   (Indices start at 1)
   This implementation computes the term directly using the recurrence.
   It is valid C11 and C++17; seq_t may be an unsigned integer type
   or an arbitrary‑precision integer class.  No external libraries are used. */
#include <stdlib.h>

static const seq_t init[6] = {85, 113, 173, 337, 565, 953};

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but guard against 0 */
        return (seq_t)0;

    if (n <= 6)
        return init[n - 1];

    /* Allocate array of size n+1 (index 0 unused) */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) abort();   /* out of memory */

    /* Initialise the first six terms */
    for (idx_t i = 1; i <= 6; ++i)
        arr[i] = init[i - 1];

    /* Compute the remaining terms */
    for (idx_t i = 7; i <= n; ++i)
        arr[i] = 6 * arr[i - 3] - arr[i - 6];

    seq_t result = arr[n];
    free(arr);
    return result;
}
