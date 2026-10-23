/*
 * A198788  Array T(k,n) read by descending antidiagonals: Last survivor
 *          positions in Josephus problem for n numbers and a count of k, n >=
 *          1, k >= 1.
 * https://oeis.org/A198788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9709, u16 n <= 9709, u32 n <= 9709, u64 n <= 9709,
 *            u128 n <= 9709, big any n
 * Helpers:   josephus
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198788: a(n) is the Josephus survivor T(k,m) where the pair (k,m)
   is obtained by listing all pairs (k,m) with k>=1,m>=1 in order of increasing
   k+m.  The array T satisfies:
   T(k,1) = 1;
   T(k,m) = ((T(k,m-1) + k - 1) mod m) + 1.                                               */
#include <stdint.h>
#include <math.h>
#include "../utils/josephus.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 */

    /* Find the smallest s such that (s-1)*s/2 >= n.  s is the sum k+m. */
    long double d = sqrtl((long double)1 + (long double)8 * (long double)n);
    idx_t s = (idx_t)((1 + d) / 2);          /* floor estimate */
    if (((s - 1) * s) / 2 < n) ++s;
    while (s > 2 && (((s - 2) * (s - 1)) / 2) >= n) --s;

    idx_t prev_total = ((s - 2) * (s - 1)) / 2;
    idx_t offset = n - prev_total - 1;       /* 0‑based offset within this anti‑diagonal */

    idx_t k = offset + 1;                    /* step size */
    idx_t m = (s - 1) - offset;              /* number of people */

    return josephus(k, m);
}
