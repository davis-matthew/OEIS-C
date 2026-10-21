/*
 * A206130  Number of (n+1) X 2 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three counterclockwise and three clockwise edge
 *          increases.
 * https://oeis.org/A206130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 8*a(n-1) - 11*a(n-2)
 *            + 4*a(n-3) for n > 5 with initial values: a(1)=256, a(2)=564,
 *            a(3)=3092, a(4)=19660, a(5)=125236 The recurrence is used
 *            directly; all calculations are done with unsigned arithmetic,
 *            relying on the fact that the intermediate sum 8*a(...
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    /* n may be large; cast to size_t for array indices */
    size_t N = (size_t)n;
    /* Allocate an array of seq_t values */
    seq_t *arr = alloc_seq_array(N);
    /* Initial values */
    arr[0] = (seq_t)256;
    if (N > 1) arr[1] = (seq_t)564;
    if (N > 2) arr[2] = (seq_t)3092;
    if (N > 3) arr[3] = (seq_t)19660;
    if (N > 4) arr[4] = (seq_t)125236;
    /* Compute further terms using the recurrence */
    for (size_t i = 5; i < N; ++i)
    {
        seq_t t = arr[i-1] * (seq_t)8 + arr[i-3] * (seq_t)4;
        t -= arr[i-2] * (seq_t)11;
        arr[i] = t;
    }
    seq_t result = arr[N-1];
    free_seq_array(arr);
    return result;
}
