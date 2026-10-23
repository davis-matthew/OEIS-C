/*
 * A186882  Alternatively squares and cubes with prime differences.
 * https://oeis.org/A186882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 23, u64 n <= 201, u128 n <= 201,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186882:  a(1)=4;  for n>1 the sequence is built
 * greedily:  if n is odd, a(n) is the smallest even perfect square
 *            greater than a(n‑1) with prime difference;
 *            if n is even, a(n) is the smallest odd perfect cube
 *            greater than a(n‑1) with prime difference.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"
#include "../utils/is_prime_big.h"

/* No static state; each call works independently, so any order
 * of calls is supported.  The algorithm is O(n²) in the worst case,
 * which is acceptable for n ≤ 10000 under the time limits.  */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n<1 */

    /* Base case a(1)=4 (even square). */
    if (n == 1)
        return (seq_t)4;

    seq_t prev = (seq_t)4;             /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t cand;
        seq_t diff;
        seq_t base;

        if (k & 1) {                   /* odd index → even square */
            base = floor_sqrt(prev);
            /* increase base until base^2 > prev */
            cand = base * base;
            if (cand <= prev) {
                base += 1;
                cand = base * base;
                if (cand <= prev) {
                    base += 1;
                    cand = base * base;
                }
            }
            /* ensure base is even */
            if (base & 1)
                base += 1;
            /* move to next even base if still too small */
            while ((base * base) <= prev)
                base += 2;
            /* search for the first even square with prime difference */
            for (;;) {
                cand = base * base;
                diff = cand - prev;
                if (is_prime_big(diff))
                    break;
                base += 2;
            }
        } else {                       /* even index → odd cube */
            base = floor_cube_root(prev);
            /* increase base until base^3 > prev */
            cand = base * base * base;
            if (cand <= prev) {
                base += 1;
                cand = base * base * base;
                if (cand <= prev) {
                    base += 1;
                    cand = base * base * base;
                }
            }
            /* ensure base is odd */
            if ((base & 1) == 0)
                base += 1;
            while ((base * base * base) <= prev)
                base += 2;
            /* search for the first odd cube with prime difference */
            for (;;) {
                cand = base * base * base;
                diff = cand - prev;
                if (is_prime_big(diff))
                    break;
                base += 2;
            }
        }
        prev = cand;
    }
    return prev;
}
