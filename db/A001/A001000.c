/*
 * A001000  a(n) = least m such that if a/b < c/d where a,b,c,d are integers
 *          in [0,n], then a/b < k/m < c/d for some integer k.
 * https://oeis.org/A001000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search for smallest m such that for each consecutive Farey
 *            fraction pair (p/q, r/s) of order n, there exists integer k with
 *            p/q < k/m < r/s
 * Range:     u8 n <= 19, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return 2;
    
    // Search for smallest m
    for (seq_t m = 2; m <= (seq_t)n * n; m++) {
        bool valid = true;
        
        // Generate Farey sequence of order n
        idx_t left_num = 0, left_den = 1;
        idx_t right_num = 1, right_den = n;
        
        while (valid) {
            // Check if there exists integer k with left_num/left_den < k/m < right_num/right_den
            // This requires an integer in open interval (left_num*m/left_den, right_num*m/right_den)
            
            // Smallest integer strictly greater than left_num*m/left_den
            seq_t lower_k = ((seq_t)left_num * m) / (seq_t)left_den + 1;
            
            // Largest integer strictly less than right_num*m/right_den
            seq_t upper_k = ((seq_t)right_num * m - 1) / (seq_t)right_den;
            
            if (lower_k > upper_k) {
                valid = false;
                break;
            }
            
            // Stop when we reach 1/1
            if (right_num == 1 && right_den == 1) break;
            
            // Generate next Farey fraction using standard algorithm
            idx_t k = (n + left_den) / right_den;
            idx_t next_num = k * right_num - left_num;
            idx_t next_den = k * right_den - left_den;
            
            left_num = right_num;
            left_den = right_den;
            right_num = next_num;
            right_den = next_den;
        }
        
        if (valid) return m;
    }
    
    return (seq_t)n * n;
}
