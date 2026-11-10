/*
 * A214228  a(n) = gcd(r,2*n+1) where r is 1 + (A143608(i-1) mod (2*n+1)) and
 *          A143608(i) is the first zero mod 2*n+1 other than i=0.
 * https://oeis.org/A214228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(1 + (A143608(i-1) mod m), m), m = 2n+1, i = least i>0
 *            with A143608(i) == 0 (mod m); A143608(0)=0, A143608(1)=1,
 *            A143608(2)=4, A143608(3)=7, A143608(k) = 6*A143608(k-2) -
 *            A143608(k-4). Computed by iterating the recurrence mod m
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 2 + 1;
    seq_t h[4];
    h[0] = 0;
    h[1] = (seq_t)1 % m;
    h[2] = (seq_t)4 % m;
    h[3] = (seq_t)7 % m;
    for (idx_t i = 1;; i++) {
        if (i >= 4) {
            h[i & 3] = ((seq_t)6 * h[(i - 2) & 3] + m - h[i & 3]) % m;
        }
        if (h[i & 3] == 0) {
            seq_t r = (seq_t)1 + h[(i - 1) & 3];
            return gcd(r, m);
        }
    }
}
