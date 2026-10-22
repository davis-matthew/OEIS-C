/*
 * A000490  Generalized Euler numbers c(4,n).
 * https://oeis.org/A000490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = A000364(n) * 16^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   power_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/power_seq.h"

#define MAX_N 10000

static seq_t euler_memo[MAX_N + 1];
static bool euler_computed[MAX_N + 1];

static seq_t get_euler(idx_t n);

static seq_t compute_euler_impl(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    
    seq_t result = 0;
    for (idx_t k = 1; k <= n; k++) {
        seq_t binom = 1;
        for (idx_t i = 1; i <= 2*k; i++) {
            binom = binom * (seq_t)(2*n - i + 1) / (seq_t)i;
        }
        
        seq_t euler_prev = get_euler(n - k);
        seq_t term = binom * euler_prev;
        
        if (k % 2 == 1) {
            result = result + term;
        } else {
            result = result - term;
        }
    }
    
    return result;
}

static seq_t get_euler(idx_t n) {
    if (n > (idx_t)MAX_N) {
        return compute_euler_impl(n);
    }
    
    if (euler_computed[n]) {
        return euler_memo[n];
    }
    
    seq_t result = compute_euler_impl(n);
    euler_memo[n] = result;
    euler_computed[n] = true;
    
    return result;
}

seq_t a(idx_t n) {
    seq_t euler = get_euler(n);
    seq_t power_of_16 = power_seq((seq_t)16, n);
    return euler * power_of_16;
}
