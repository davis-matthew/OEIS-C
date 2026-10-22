/*
 * A001312  Number of ways of making change for n cents using coins of 1, 2,
 *          5, 10, 50, 100 cents.
 * https://oeis.org/A001312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{100i+50j+10k <= n} f(n-100i-50j-10k), where f(m) =
 *            Sum_{t=0..floor(m/5)} (floor((m-5t)/2) + 1) is the number of
 *            ways to make m from coins 1, 2, 5 (evaluated in closed form).
 *            G.f.: 1/((1-x)*(1-x^2)*(1-x^5)*(1-x^10)*(1-x^50)*(1-x^100))
 * Range:     u8 n <= 44, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ways125
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ways125.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    for (uint64_t r1 = (uint64_t)n;; r1 -= 100) {
        for (uint64_t r2 = r1;; r2 -= 50) {
            uint64_t part = 0;
            for (uint64_t r3 = r2;; r3 -= 10) {
                part += ways125(r3);
                if (r3 < 10) break;
            }
            result += (seq_t)part;
            if (r2 < 50) break;
        }
        if (r1 < 100) break;
    }
    return result;
}
