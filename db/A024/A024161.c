/*
 * A024161  Number of integer-sided triangles with sides a,b,c, a < b < c,
 *          a+b+c = n such that a,b,c are pairwise relatively prime.
 * https://oeis.org/A024161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (a,b,c) | 1≤a<b<c, a+b+c = n, a+b>c,
 *            gcd(a,b)=gcd(b,c)=gcd(a,c)=1 }
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of pairwise coprime integer‑sided triangles with
   perimeter n, sides a<b<c and triangle inequalities satisfied. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* seq_t and idx_t are defined by the build environment */
seq_t a(idx_t n)
{
    /* Perimeter must be at least 6 (1+2+3). */
    if (n < 6)
        return 0;

    seq_t count = 0;
    /* a cannot exceed floor((n-3)/3) */
    idx_t a_lim = (n - 3) / 3;
    for (idx_t a = 1; a <= a_lim; ++a) {
        /* Minimum allowed b */
        idx_t min_b = a + 1;
        /* Lower bound from triangle inequality: a + b > n/2
           Equivalent to 2*(a+b) > n, i.e. b > (n-2a)/2.
           For integers: floor((n-2a)/2) + 1 */
        idx_t lower_b = ((n - 2*a) / 2) + 1;
        if (lower_b < min_b)
            lower_b = min_b;

        /* Upper bound from c > b: b < (n-a)/2
           Integer upper bound: floor((n-a-1)/2) */
        idx_t max_b = (n - a - 1) / 2;

        if (lower_b > max_b)
            continue;

        for (idx_t b = lower_b; b <= max_b; ++b) {
            idx_t c = n - a - b;           /* c is automatically > b */
            seq_t A = (seq_t)a;
            seq_t B = (seq_t)b;
            seq_t C = (seq_t)c;
            if (gcd(A, B) != 1)
                continue;
            if (gcd(A, C) != 1)
                continue;
            if (gcd(B, C) != 1)
                continue;
            ++count;
        }
    }
    return count;
}
