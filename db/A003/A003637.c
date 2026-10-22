/*
 * A003637  Number of classes per genus in quadratic field with discriminant
 *          -4n+1.
 * https://oeis.org/A003637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(-D)/2^(w(D)-1), where -D is the n-th fundamental
 *            discriminant with D == 3 (mod 4) squarefree (D =
 *            3,7,11,15,19,23,31,35,...), h is the class number (counted by
 *            enumerating reduced forms) and w(D) = number of distinct prime
 *            factors of D (number of genera = 2^(w-1))
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   omega_odd, sqfree_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega_odd.h"
#include "../utils/sqfree_odd.h"

static int64_t class_number(int64_t D) {
    /* reduced forms (a,b,c), b^2 - 4ac = -D, |b| <= a <= c,
       b >= 0 if |b| == a or a == c */
    int64_t h = 0;
    for (int64_t a = 1; 3 * a * a <= D; a++) {
        for (int64_t b = -a + 1; b <= a; b += 1) {
            if (((b % 2) + 2) % 2 != 1) continue;
            int64_t num = b * b + D;
            if (num % (4 * a) != 0) continue;
            int64_t c = num / (4 * a);
            if (c < a) continue;
            if (a == c && b < 0) continue;
            h++;
        }
    }
    return h;
}

seq_t a(idx_t n) {
    idx_t cnt = 0;
    int64_t D = -1;
    for (;;) {
        D += 4;
        if (sqfree_odd(D)) {
            cnt++;
            if (cnt == n) break;
        }
    }
    int64_t h = class_number(D);
    int w = omega_odd(D);
    int64_t g = (int64_t)1 << (w - 1);
    return (seq_t)(h / g);
}
