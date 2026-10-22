/*
 * A004081  a(n) = n-th positive integer such that only one integer lies
 *          between exp(s(m)) and exp(s(m+1)), where s(m) = 1 + 1/2 + 1/3 + .
 *          . . + 1/m.
 * https://oeis.org/A004081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search for m such that exactly one integer k lies in (exp(H_m),
 *            exp(H_{m+1})) where H_m = 1 + 1/2 + ... + 1/m
 * Range:     u8 n <= 56, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>


seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 1;
    double h = 1.0;
    
    while (count < n) {
        double h_next = h + 1.0 / (m + 1);
        
        double exp_h = exp(h);
        double exp_h_next = exp(h_next);
        
        long long smallest = (long long)floor(exp_h) + 1;
        long long largest = (long long)floor(exp_h_next);
        if (exp_h_next == (double)largest) {
            largest--;
        }
        
        if (smallest == largest) {
            count++;
            if (count == n) {
                return (seq_t)m;
            }
        }
        
        m++;
        h = h_next;
    }
    
    return 0;
}
