/*
 * A003333  Numbers that are the sum of 10 positive cubes.
 * https://oeis.org/A003333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 99 for n > 275. For n <= 275, a(n) is the n-th
 *            positive integer expressible as the sum of 10 positive cubes
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   generate_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/generate_sums.h"

seq_t a(idx_t n) {
    static bool computed = false;
    static bool reachable[375];
    
    if (!computed) {
        computed = true;
        memset(reachable, false, sizeof(reachable));
        generate_sums(0, 0, 1, reachable);
    }
    
    if (n > 275) {
        return (seq_t)n + 99;
    }
    
    long long count = 0;
    for (int k = 1; k <= 374; k++) {
        if (reachable[k]) {
            count++;
            if (count == (long long)n) {
                return (seq_t)k;
            }
        }
    }
    
    return 0;
}
