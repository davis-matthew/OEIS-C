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
 * Helpers:   proper_divisor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/proper_divisor_sum.h"

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
