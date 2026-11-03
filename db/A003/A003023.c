/*
 * A003023  "Length" of aliquot sequence for n.
 * https://oeis.org/A003023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Length of aliquot sequence for n. Start with n, repeatedly apply
 *            s(n) = sigma(n) - n (sum of proper divisors). Stop when reaching
 *            1 or entering a cycle. Return trajectory length
 * Range:     u8 n <= 29, u16 n <= 137, u32 n <= 137, u64 n <= 275,
 *            u128 n <= 275, big any n
 * Validated: standard, pass, 118 terms through n = 261 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t proper_divisor_sum(seq_t x) {
    if (x <= 1) return 0;
    
    seq_t sum = 1;
    seq_t i = 2;
    
    while (i * i <= x) {
        if (x % i == 0) {
            seq_t complement = x / i;
            sum += i;
            if (i != complement) {
                sum += complement;
            }
        }
        i++;
    }
    
    return sum;
}

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    
    if (n == 1) {
        return 0;
    }
    
    seq_t seen[100000];
    int seen_count = 0;
    seen[seen_count++] = n;
    seq_t count = 1;
    
    while (seen_count < 100000) {
        seq_t next = proper_divisor_sum(n);
        
        if (next == 1) {
            return count;
        }
        
        bool found = false;
        for (int i = 0; i < seen_count; i++) {
            if (seen[i] == next) {
                found = true;
                break;
            }
        }
        
        if (found) {
            return count;
        }
        
        seen[seen_count++] = next;
        n = next;
        count++;
    }
    
    return count;
}
