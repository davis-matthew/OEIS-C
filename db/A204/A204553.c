/*
 * A204553  Symmetric matrix: f(i,j)=floor[(2i+2j+2)/4]-floor[(i+j+1)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value on the antidiagonal of the symmetric * matrix
 *            f(i,j)=⌊(2i+2j+2)/4⌋−⌊(i+j+1)/4⌋. * Let s be the unique integer
 *            satisfying * (s-1)s/2 ≥ n > (s-2)(s-1)/2 . * Then
 *            a(n)=⌊(s+1)/2⌋−⌊(s+1)/4⌋. * The function determines s by a
 *            binary search in O(log n) time * and returns the above value
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t
find_s(idx_t n)
{
    /* Find minimal s with (s-1)*s/2 >= n. */
    idx_t low = 1;
    idx_t high = 1;
    /* Grow high until it satisfies the inequality. */
    while ((unsigned __int128)(high - 1) * high / 2 < n) {
        high <<= 1;
    }
    /* Binary search for the minimal s. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)(mid - 1) * mid / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Offset 1: the sequence is defined for n >= 1. */
    if (n == 0)
        return 0;  /* Undefined, but return 0 for safety. */

    idx_t s = find_s(n);
    seq_t t = (seq_t)(s + 1);                /* t = s + 1  */
    seq_t v1 = t / 2;                        /* ⌊t/2⌋     */
    seq_t v2 = t / 4;                        /* ⌊t/4⌋     */
    return v1 - v2;                          /* a(n)       */
}
