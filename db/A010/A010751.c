/*
 * A010751  Up once, down twice, up three times, down four times, ...
 * https://oeis.org/A010751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the position after n steps of a walk that moves +1 once,
 *            -1 twice, +1 three times, -1 four times, ... This implementation
 *            uses only integer arithmetic
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for an unsigned __int128 value;
   returns floor(sqrt(n)). */
static inline unsigned __int128 isqrt128(unsigned __int128 n)
{
    /* Binary search for the largest x such that x*x <= n. */
    unsigned __int128 low = 0, high = (unsigned __int128)1 << 64;
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Base case: before any step. */
    if (n == 0)
        return (seq_t)0;

    /* Compute floor(sqrt(1+8n)). */
    unsigned __int128 val = (unsigned __int128)n * 8 + 1;
    unsigned __int128 sq = isqrt128(val);
    idx_t g = (idx_t)((sq - 1) / 2);          /* floor((sqrt(1+8n)-1)/2) */

    /* Adjust g so that g*(g+1)/2 >= n. */
    __int128 tri = (__int128)g * (g + 1) / 2;
    if (tri < (__int128)n)
        ++g;

    /* Steps taken before group g. */
    __int128 sum_prev = (__int128)(g - 1) * g / 2;
    idx_t m = (idx_t)((__int128)n - sum_prev);   /* 1 <= m <= g */

    /* Position at the start of group g. */
    idx_t prev = g - 1;
    seq_t start;
    if ((prev & 1) == 0)          /* prev even */
        start = -(seq_t)(prev / 2);
    else                          /* prev odd */
        start = (seq_t)((prev + 1) / 2);

    /* Direction of group g: +1 for odd g, -1 for even g. */
    int sign = (g & 1) ? 1 : -1;

    /* Final position. */
    return start + (seq_t)sign * (seq_t)m;
}
