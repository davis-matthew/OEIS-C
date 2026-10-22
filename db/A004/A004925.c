/*
 * A004925  a(n) = floor(n*phi^10), where phi is the golden ratio, A001622.
 * https://oeis.org/A004925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * (123 + 55*sqrt(5)) / 2)
 * Range:     u8 n <= 2, u16 n <= 532, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long long a_val = (long long)n * 123LL;
    long long b_val = (long long)n * 55LL;
    
    // Estimate using floating point
    long double sqrt5 = sqrtl(5.0L);
    long double numerator = (long double)a_val + (long double)b_val * sqrt5;
    long double result = numerator / 2.0L;
    long long k = (long long)floorl(result);
    
    // Adjust k to ensure it's the floor of (a_val + b_val*sqrt(5)) / 2
    
    // Decrease k if 2*k > a_val + b_val*sqrt(5)
    while (1) {
        long long diff = 2LL * k - a_val;
        if (diff > 0) {
            unsigned long long diff_sq = (unsigned long long)diff * (unsigned long long)diff;
            unsigned long long b_sq = (unsigned long long)b_val * (unsigned long long)b_val;
            if (diff_sq > 5ULL * b_sq) {
                k--;
                continue;
            }
        }
        break;
    }
    
    // Increase k if 2*(k+1) <= a_val + b_val*sqrt(5)
    while (1) {
        long long diff = 2LL * k + 2LL - a_val;
        if (diff <= 0) {
            k++;
            continue;
        }
        unsigned long long diff_sq = (unsigned long long)diff * (unsigned long long)diff;
        unsigned long long b_sq = (unsigned long long)b_val * (unsigned long long)b_val;
        if (diff_sq <= 5ULL * b_sq) {
            k++;
            continue;
        }
        break;
    }
    
    return (seq_t)k;
}
