/*
 * A075362  Triangle read by rows with the n-th row containing the first n
 *          multiples of n.
 * https://oeis.org/A075362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   Let t = floor((sqrt(8*n-7)-1)/2), i = n - t*(t+1)/2. Then a(n) =
 *            (t+1) * i. (Row-by-row enumeration of the triangle T(n,k)=n*k)
 * Range:     u8 n <= 135, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a unsigned __int128 number.
   Returns the floor of sqrt(val). */
static unsigned __int128 isqrt128(unsigned __int128 val)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64;   /* sqrt(2^128-1) < 2^64 */
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid == (unsigned __int128)0) {
            low = mid + 1;
            continue;
        }
        /* Avoid overflow: if val / mid < mid then mid*mid > val */
        if (val / mid < mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* n == 0 is outside the defined domain (offset 1); return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    unsigned __int128 val = (unsigned __int128)8 * (unsigned __int128)n - 7;
    unsigned __int128 sqrt_val = isqrt128(val);
    idx_t t = (idx_t)((sqrt_val - 1) / 2);          /* row index - 1 */
    idx_t sum_before = t * (t + 1) / 2;             /* sum of first t integers */
    idx_t i = n - sum_before;                       /* position within the row */
    seq_t result = (seq_t)(t + 1) * (seq_t)i;
    return result;
}
