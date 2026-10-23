/*
 * A048766  Integer part of cube root of n. Or, number of cubes <= n. Or, n
 *          appears 3n^2 + 3n + 1 times.
 * https://oeis.org/A048766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊n^(1/3)⌋ = integer part of the cube root of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Binary search for the largest k with k^3 <= n.
       The upper bound 1 << 22 exceeds the cube root of UINT64_MAX. */
    idx_t low = 0;
    idx_t high = (idx_t)1 << 22;              /* 2^22 ≈ 4 194 304 */

    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;    /* upper mid to avoid infinite loop */

        /* mid^2 fits in idx_t (mid <= 2^22).  Use division to test mid^3 <= n
         * without overflow: mid <= n / (mid*mid). */
        idx_t mid2 = mid * mid;
        if (mid == 0 || mid <= n / mid2)
            low = mid;
        else
            high = mid - 1;
    }

    return (seq_t)low;
}
