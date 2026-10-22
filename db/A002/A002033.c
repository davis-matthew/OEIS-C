/*
 * A002033  Number of perfect partitions of n.
 * https://oeis.org/A002033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = number of ordered factorizations of n+1. Computed via
 *            f(m) = sum over divisors d > 1 of m: f(m/d), with f(1) = 1
 * Range:     u8 n <= 190, u16 n <= 8638, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_cap = 0;

static seq_t ordered_factorizations(idx_t m) {
    if (m == 1) return 1;
    
    // Ensure memo array is large enough
    if (m >= memo_cap) {
        idx_t old_cap = memo_cap;
        idx_t new_cap = m * 2 + 1;
        seq_t *new_memo = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (memo) {
            memcpy(new_memo, memo, old_cap * sizeof(seq_t));
            free(memo);
        }
        memset(new_memo + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
        memo = new_memo;
        memo_cap = new_cap;
    }
    
    if (memo[m] != 0) return memo[m];
    
    seq_t result = 0;
    
    // Sum over all divisors d > 1 of m
    for (idx_t d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            result += ordered_factorizations(m / d);
            idx_t other_d = m / d;
            if (other_d != d) {
                result += ordered_factorizations(m / other_d);
            }
        }
    }
    // m itself is always a divisor > 1
    result += ordered_factorizations(1);
    
    memo[m] = result;
    return result;
}

seq_t a(idx_t n) {
    return ordered_factorizations(n + 1);
}
