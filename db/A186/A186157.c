/*
 * A186157  Rank of 2n^2 when {i^3: i>=1} and {2j^2: j>=1} are jointly ranked
 *          with i^3 before 2j^2 when i^3=2j^2. Complement of A186156.
 * https://oeis.org/A186157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rank of the number 2n² among all numbers of the form i³ or 2j².
 * The ordering is ascending, and when a cube equals a twice‑square,
 * the cube appears first.  a(n) gives the position of 2n² in this
 * ordered list, with a(1) = 2. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) for x ≥ 0, where x fits in idx_t. */
static inline idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Largest integer i such that i³ < x (strict inequality), for x ≤ 2^64. */
static inline idx_t icbrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        unsigned __int128 mid3 = (unsigned __int128)mid * mid * mid;
        if (mid3 < x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Compute the value 2n²; it is guaranteed to fit in idx_t for the
     * test range (n up to about 10⁴). */
    idx_t n0 = n;
    idx_t x_idx = n0 * n0 * 2;            /* 2 * n² as idx_t               */

    /* Count cubes strictly less than x. */
    idx_t cube_cnt = icbrt_idx(x_idx);    /* i³ < x */

    /* Count twice squares strictly less than x.
     * Condition: 2·j² < x  <=>  j² < x/2  <=>  j² ≤ floor((x-1)/2)  */
    idx_t double_cnt = isqrt_idx((x_idx - 1) / 2);

    /* Check whether x itself is also a cube.  If so, the cube
     * precedes the double‑square entry with the same value. */
    idx_t i = cube_cnt + 1;
    unsigned __int128 i3 = (unsigned __int128)i * i * i;
    bool is_cube = (i3 == (unsigned __int128)x_idx);

    /* Rank of 2n²:
     *   number of elements strictly less than x
     * + 1 for the double‑square itself
     * + 1 more if a cube of the same value appears before it.   */
    seq_t rank = (seq_t)cube_cnt + double_cnt + 1;
    if (is_cube)
        rank += 1;
    return rank;
}
