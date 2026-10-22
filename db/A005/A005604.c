/*
 * A005604  a(n) = a(n-1)! + a(n-2)!.
 * https://oeis.org/A005604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)! + a(n-2)!, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   fact_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fact_small.h"

seq_t a(idx_t n) {
    static seq_t cache[16];
    static bool have[16];
    seq_t prev2, prev1, cur;
    idx_t i;

    if (n < 16 && have[n]) {
        return cache[n];
    }
    if (n < 2) {
        return (seq_t)1;
    }

    prev2 = (seq_t)1;
    prev1 = (seq_t)1;
    cur = (seq_t)1;
    for (i = 2; i <= n; i++) {
        if (i < 16 && have[i]) {
            cur = cache[i];
        } else {
            cur = fact_small(prev1) + fact_small(prev2);
            if (i < 16) {
                cache[i] = cur;
                have[i] = true;
            }
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
