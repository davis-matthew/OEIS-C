/*
 * A002097  Numbers that are not the sum of 3 nonzero triangular numbers.
 * https://oeis.org/A002097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   count numbers not representable as sum of 3 nonzero triangular
 *            numbers
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-02)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isqrt_seq.h"

static bool is_triangular(seq_t num) {
    if (num <= 0) return false;
    
    /* n is triangular iff 8n+1 is a perfect square with odd sqrt */
    seq_t disc = 8 * num + 1;
    seq_t sq = isqrt_seq(disc);
    
    return sq * sq == disc && (sq & 1);
}

static bool is_sum_of_three_triangular(seq_t k) {
    if (k < 3) return false;
    
    seq_t limit = isqrt_seq(2 * k) + 1;
    
    for (seq_t i = 1; i <= limit; i++) {
        seq_t t1 = i * (i + 1) / 2;
        if (t1 >= k) break;
        
        for (seq_t j = 1; j <= limit; j++) {
            seq_t t2 = j * (j + 1) / 2;
            if (t1 + t2 >= k) break;
            
            if (is_triangular(k - t1 - t2)) {
                return true;
            }
        }
    }
    
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cand = 1;
    
    while (cand <= (seq_t)100000) {
        if (!is_sum_of_three_triangular(cand)) {
            count++;
            if (count == n) return cand;
        }
        cand++;
    }
    
    return 0;
}
