/*
 * A007737  Period of repeating digits of 1/n in base 6.
 * https://oeis.org/A007737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = multiplicative order of 6 modulo n (after removing
 *            factors of 2 and 3)
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    
    while (n_val % 2 == 0) {
        n_val /= 2;
    }
    while (n_val % 3 == 0) {
        n_val /= 3;
    }
    
    if (n_val == 1) {
        return 1;
    }
    
    unsigned long long power = 6 % (unsigned long long)n_val;
    seq_t order = 1;
    
    while (power != 1) {
        power = (power * 6) % (unsigned long long)n_val;
        order++;
    }
    
    return order;
}
