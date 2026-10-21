/*
 * A226341  Number of nondecreasing -n..n vectors of length 2 whose dot
 *          product with some other -n..n vector equals 2.
 * https://oeis.org/A226341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 207, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   ceil_div_pos, floor_div_pos, gcd_ext, update_bounds
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = number of nondecreasing vectors (x,y) with -n ≤ x ≤ y ≤ n
   such that there exists integers u,v with -n ≤ u,v ≤ n and x*u + y*v = 2.  */
/* The implementation enumerates all (x,y) and uses extended gcd to decide
   whether a bounded solution (u,v) exists. */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/ceil_div_pos.h"
#include "../utils/floor_div_pos.h"
#include "../utils/gcd_ext.h"
#include "../utils/update_bounds.h"

/* Standard Euclidean gcd. */
static inline long long gcd_ll(long long a, long long b) {
    while (b) {
        long long t = a % b;
        a = b; b = t;
    }
    return a >= 0 ? a : -a;
}

seq_t a(idx_t n) {
    long long N = (long long)n;          /* small enough to fit in signed long long */
    unsigned long long count = 0;

    for (long long x = -N; x <= N; ++x) {
        for (long long y = x; y <= N; ++y) {
            int ok = 0;

            if (x == 0 && y == 0) {
                ok = 0;                            /* cannot reach dot product 2 */
            } else if (x == 0) {                    /* y != 0 */
                if (y != 0 && (2 % y) == 0) {
                    long long v = 2 / y;
                    if (llabs(v) <= N) ok = 1;
                }
            } else if (y == 0) {                    /* x != 0 */
                if (x != 0 && (2 % x) == 0) {
                    long long u = 2 / x;
                    if (llabs(u) <= N) ok = 1;
                }
            } else {                                /* both non‑zero */
                long long absx = llabs(x);
                long long absy = llabs(y);
                long long g    = gcd_ll(absx, absy);
                if ((2 % g) == 0) {
                    long long s, t;
                    gcd_ext(absx, absy, &s, &t);      /* absx*s + absy*t = g */
                    s *= (x >= 0) ? 1 : -1;
                    t *= (y >= 0) ? 1 : -1;
                    long long factor = 2 / g;
                    long long u0 = s * factor;
                    long long v0 = t * factor;
                    long long A = y / g;          /* coefficient for k in u */
                    long long B = -x / g;         /* coefficient for k in v */

                    long long k_low  = LLONG_MIN;
                    long long k_high = LLONG_MAX;

                    if (!update_bounds(A, u0, N, &k_low, &k_high)) continue;
                    if (!update_bounds(B, v0, N, &k_low, &k_high)) continue;

                    if (k_low <= k_high) ok = 1;
                }
            }
            if (ok) ++count;
        }
    }
    return (seq_t)count;
}
