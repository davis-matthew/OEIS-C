/*
 * A000443  Numbers that are the sum of 2 squares in exactly 3 ways.
 * https://oeis.org/A000443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are sum of 2 squares in exactly 3 ways
 * Range:     u8 none, u16 n <= 677, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9033 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static idx_t terms_count = 0;
    static seq_t max_checked = 0;
    
    while (terms_count < n) {
        // Expand sieve limit
        seq_t limit = (max_checked == 0) ? 100000 : max_checked + max_checked;
        
        // Count representations using sieve
        unsigned char *count = (unsigned char *)malloc((size_t)(limit + 1));
        memset(count, 0, (size_t)(limit + 1));
        
        for (seq_t a = 0; a * a <= limit; a++) {
            for (seq_t b = a; a * a + b * b <= limit; b++) {
                seq_t sum = a * a + b * b;
                if (count[(size_t)sum] < 10) {
                    count[(size_t)sum]++;
                }
            }
        }
        
        // Find new terms with exactly 3 representations
        seq_t *new_terms = mem_new(seq_t, (size_t)(limit - max_checked));
        idx_t new_count = 0;
        for (seq_t i = max_checked + 1; i <= limit; i++) {
            if (count[(size_t)i] == 3) {
                new_terms[(size_t)new_count] = i;
                new_count++;
            }
        }
        
        // Merge with existing terms
        if (terms == NULL) {
            terms = new_terms;
            terms_count = new_count;
        } else {
            seq_t *merged = mem_new(seq_t, (size_t)(terms_count + new_count));
            mem_copy(merged, terms, (size_t)terms_count * sizeof(seq_t));
            mem_copy(merged + (size_t)terms_count, new_terms, (size_t)new_count * sizeof(seq_t));
            mem_free(terms);
            mem_free(new_terms);
            terms = merged;
            terms_count += new_count;
        }
        
        max_checked = limit;
        free(count);
    }
    
    return terms[(size_t)(n - 1)];
}
