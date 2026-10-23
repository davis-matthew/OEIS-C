/*
 * A177063  Number of Pythagorean triangles with area n.
 * https://oeis.org/A177063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 9656, u64 n <= 9656,
 *            u128 n <= 9656, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = number of distinct integer right triangles with area n.
   For a positive integer n this is the number of unordered pairs of
   positive integers (x,y) such that
     x * y = 2n   and   x² + y² is a perfect square.
   The function iterates over all divisors d of 2n up to √(2n),
   checks the corresponding co‑divisor e = 2n/d, and counts those
   pairs for which d² + e² is a square.
   The calculation uses only integer arithmetic and a simple
   binary‑search integer square root.
*/

#include <stdbool.h>

/* Integer square root: returns floor(sqrt(x)) */
static seq_t isqrt_seq(seq_t x)
{
    if (x == 0) return 0;
    seq_t low = 0;
    seq_t high = x;
    seq_t mid, mid2;
    while (low <= high) {
        mid = (low + high) / 2;
        mid2 = mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;   /* floor */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, but harmless */

    /* two_n = 2 * n, fits in idx_t for n up to at least 10^9 */
    idx_t two_n = n * 2;

    /* compute limit = floor(sqrt(two_n)) without external helpers */
    idx_t limit = 0;
    while ((limit + 1) * (limit + 1) <= two_n) limit++;

    seq_t count = 0;

    for (idx_t d = 1; d <= limit; ++d) {
        if (two_n % d != 0) continue;  /* d must divide 2n */

        idx_t e = two_n / d;
        if (d > e) continue;           /* ensure each unordered pair once */

        seq_t d_sq = (seq_t)d * (seq_t)d;
        seq_t e_sq = (seq_t)e * (seq_t)e;
        seq_t sum  = d_sq + e_sq;

        seq_t r = isqrt_seq(sum);
        if (r * r == sum) {
            count += 1;
        }
    }
    return count;
}
