/*
 * A007950  Binary sieve: delete every 2nd number, then every 4th, 8th, etc.
 * https://oeis.org/A007950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Binary sieve: delete every 2^k-th number for k=1,2,3,... from remaining sequence */

static bool is_in_sequence(seq_t m) {
    seq_t pos = m;
    seq_t power = 2;
    
    while (power <= pos) {
        if (pos % power == 0) {
            return false;
        }
        pos = pos - pos / power;
        power *= 2;
    }
    
    return true;
}

static seq_t count_up_to(seq_t m) {
    seq_t count = 0;
    seq_t i;
    for (i = 1; i <= m; i++) {
        if (is_in_sequence(i)) {
            count++;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t left = 1;
    seq_t right = n_val * 4;
    
    while (left < right) {
        seq_t mid = left + (right - left) / 2;
        seq_t count = count_up_to(mid);
        
        if (count < n_val) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return left;
}
