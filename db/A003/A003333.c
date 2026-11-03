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
 * Validated: standard, pass, 77 terms through n = 9673 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

static void generate_sums(int depth, int current_sum, int min_val, bool *reachable) {
    if (depth == 10) {
        if (current_sum <= 374) {
            reachable[current_sum] = true;
        }
        return;
    }
    
    int remaining = 10 - depth;
    if (current_sum + remaining * 1 > 374) return;
    
    for (int v = min_val; v <= 7; v++) {
        int cube = v * v * v;
        int new_sum = current_sum + cube;
        
        if (new_sum > 374) break;
        
        generate_sums(depth + 1, new_sum, v, reachable);
    }
}

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
