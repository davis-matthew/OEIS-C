/*
 * A108261  2nd order recursive series having the property that the product of
 *          any two adjacent terms is a triangular number, T(b) = b(b+1)/2
 *          where b equals term a(n) of related series A108262.
 * https://oeis.org/A108261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) – a(n-2) – 4, a(0)=2, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation buffer and its size information */
static seq_t *dp      = NULL;   /* sequence values */
static idx_t  capacity = 0;     /* capacity of allocated buffer */
static idx_t  size     = 0;     /* number of values currently stored */

/* Grow the buffer if necessary and compute all missing terms up to 'needed'. */
static void ensure_capacity(idx_t needed)
{
    /* No work if we already have everything needed */
    if (size > 0 && needed < size) return;

    /* Allocate or enlarge the buffer to cover 'needed' */
    if (capacity <= needed) {
        idx_t newCap = capacity ? capacity << 1 : 16;
        while (newCap <= needed) newCap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, newCap * sizeof(seq_t));
        dp = tmp;
        capacity = newCap;
    }

    /* Compute new terms from the current size up to 'needed' */
    for (idx_t i = size; i <= needed; ++i) {
        if (i == 0)          dp[0] = (seq_t)2;
        else if (i == 1)     dp[1] = (seq_t)3;
        else
            dp[i] = (seq_t)6 * dp[i-1] - dp[i-2] - (seq_t)4;
    }
    size = needed + 1;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return dp[n];
}
