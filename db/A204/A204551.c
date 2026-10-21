/*
 * A204551  Symmetric matrix: f(i,j)=floor[(2i+2j+1)/4]-floor[(i+j)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((s+2)/4), where s is the smallest integer ≥2 such
 *            that * (s-1)*s/2 ≥ n. Equivalently, s = ceil((1+sqrt(1+8·n))/2).
 *            * The sequence lists the values floor((s+2)/4) repeated exactly
 *            (s-1) times * for each s = 2,3,4,..., which corresponds to the
 *            constant–antidiagonal * entries of the mat...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>               /* for UINT64_MAX */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;    /* sequence is defined for n ≥ 1 */

    /* Binary search for the minimal s with (s‑1)*s/2 ≥ n. */
    idx_t low  = 1;                /* s will eventually be ≥ 2 */
    idx_t high = 2;

    /* Expand the search window until it surely contains the answer. */
    while (1) {
        __int128 cand = (__int128)(high - 1) * (__int128)high / 2;
        if (cand >= n) break;
        if (high > UINT64_MAX / 2) { /* avoid overflow of high */
            high = UINT64_MAX;
            break;
        }
        high += high;               /* double the upper bound */
    }

    /* Standard binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128)(mid - 1) * (__int128)mid / 2;
        if (val >= n)
            high = mid;
        else
            low  = mid + 1;
    }

    idx_t s = low;                 /* (s‑1)*s/2 ≥ n and minimal */

    /* Compute the value for this antidiagonal. */
    return (seq_t)((s + 2) / 4);
}
