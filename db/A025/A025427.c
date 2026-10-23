/*
 * A025427  Number of partitions of n into 3 nonzero squares.
 * https://oeis.org/A025427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute a(n) = Number of partitions of n into 3 nonzero squares.
 * The function counts unordered triples (a,b,c) of positive squares
 * such that a + b + c = n and a ≤ b ≤ c.  It uses an integer
 * square–root routine and a boolean map of squares up to n.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return ⌊√n⌋ for a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        if (mid != 0 && mid > n / mid)
            high = mid - 1;
        else
            low  = mid + 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* n < 3 cannot be expressed as the sum of three positive squares */
    if (n < 3)
        return 0;

    /* Pre‑compute the largest square root needed. */
    idx_t maxRoot = isqrt_u64(n);
    if (maxRoot < 1)
        return 0;

    /* Allocate a boolean array to test whether a number <= n is a square. */
    size_t arraySize = (size_t)(n + 1);
    uint8_t *isSquare = (uint8_t *)calloc(arraySize, sizeof(uint8_t));
    if (!isSquare)
        return 0;   /* allocation failure -> 0 (values are small). */

    /* Mark all perfect squares up to n. */
    for (idx_t r = 1; r <= maxRoot; ++r)
    {
        idx_t sq = r * r;
        if (sq <= n)
            isSquare[sq] = 1;
    }

    seq_t count = 0;

    /* Enumerate unordered triples (a,b,c) with a ≤ b ≤ c. */
    for (idx_t m1 = 1; m1 <= maxRoot; ++m1)
    {
        idx_t a = m1 * m1;
        if (a > n)
            break;

        for (idx_t m2 = m1; m2 <= maxRoot; ++m2)
        {
            idx_t b = m2 * m2;
            idx_t sum_ab = a + b;
            if (sum_ab > n)
                break;

            idx_t c = n - sum_ab;
            if (c < b)
                continue;            /* ensure nondecreasing order */

            if (c <= n && isSquare[c])
                count = count + 1;
        }
    }

    free(isSquare);
    return count;
}
