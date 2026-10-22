/*
 * A005844  a(n+1) = 2^a(n) - 1.
 * https://oeis.org/A005844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n+1) = 2^a(n) - 1, a(0) = 3
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 2, i64 n <= 2, i128 n <= 3,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/A001832_pow2.h"

seq_t a(idx_t n) {
    static seq_t memo[4];
    static bool computed[4];
    
    if (n >= 4) {
        return 0;
    }
    
    if (computed[n]) {
        return memo[n];
    }
    
    seq_t result;
    if (n == 0) {
        result = 3;
    } else {
        seq_t prev = a(n - 1);
        result = A001832_pow2((idx_t)prev) - 1;
    }
    
    computed[n] = true;
    memo[n] = result;
    return result;
}
