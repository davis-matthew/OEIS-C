/*
 * A003668  a(n) is smallest number which is uniquely a(j)+a(k), j<k.
 * https://oeis.org/A003668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Ulam-type sequence; compute iteratively by finding smallest
 *            number expressible uniquely as sum of two distinct earlier
 *            terms. For n > 7, a(n+26) = a(n) + 126
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    
    // For n > 33, use the recurrence a(n+26) = a(n) + 126
    if (n > 33) {
        idx_t m = (n - 8) / 26;
        idx_t r = (n - 8) % 26;
        seq_t base_val = a(8 + r);
        return base_val + (seq_t)m * (seq_t)126;
    }
    
    // Initialize on first call
    if (seq == NULL) {
        seq = (seq_t *)malloc(34 * sizeof(seq_t));
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)7;
        seq_len = 2;
    }
    
    // Build sequence up to index n
    while (seq_len < n) {
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;
        
        while (true) {
            // Count representations as seq[j] + seq[k], j < k
            int count = 0;
            for (idx_t j = 0; j < seq_len; j++) {
                for (idx_t k = j + 1; k < seq_len; k++) {
                    if (seq[j] + seq[k] == candidate) {
                        count++;
                    }
                }
            }
            
            // Add if uniquely representable
            if (count == 1) {
                seq[seq_len++] = candidate;
                break;
            }
            
            candidate++;
        }
    }
    
    return seq[n - 1];
}
