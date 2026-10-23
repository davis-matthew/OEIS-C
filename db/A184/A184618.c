/*
 * A184618  a(n) = floor(n*r + h), where r=sqrt(2) and h=1/3; complement of
 *          A184619.
 * https://oeis.org/A184618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(2) + 1/3)
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = floor(n*r + h) where r = sqrt(2), h = 1/3   */

seq_t a(idx_t n)
{
    /* offset = 1 : a(1) is the first term.  The function must handle
       n >= 1; return 0 for n == 0, which is never used by tests. */
    if (n == 0)
        return 0;

    /* binary search for the unique integer k that satisfies
       (3k-1)^2 <= 18*n^2 < (3k+2)^2   */
    idx_t low  = 0;
    idx_t high = n * 2 + 1;          /* a loose upper bound, always safe */
    __uint128_t target = (__uint128_t)18 * n * n;

    while (low <= high) {
        idx_t mid = (low + high) >> 1;

        __uint128_t left  = (__uint128_t)3 * mid - 1;   /* 3k - 1  */
        __uint128_t right = (__uint128_t)3 * mid + 2;   /* 3k + 2  */

        __uint128_t leftSq  = left  * left;
        __uint128_t rightSq = right * right;

        if (leftSq <= target && target < rightSq) {
            return (seq_t)mid;   /* found the correct k */
        } else if (target < leftSq) {
            if (mid == 0) break;   /* protect against underflow */
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* Fallback – should never be reached for valid inputs.  */
    return (seq_t)low;
}
