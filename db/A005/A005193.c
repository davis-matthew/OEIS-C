/*
 * A005193  a(n) is the number of alpha-labelings of graphs with n edges.
 * https://oeis.org/A005193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   if n is even: a(n) = 2*Sum_{j=1..floor(n/2)} j!^2*j^(n-2*j), if
 *            n is odd: a(n) = 2*Sum_{j=1..floor(n/2)} j!^2*j^(n-2*j) +
 *            ((n+1)/2)!*((n-1)/2)!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t sum = 0;
    seq_t j_fact = 1;
    
    for (idx_t j = 1; j <= n / 2; j++) {
        j_fact *= (seq_t)j;
        idx_t exponent = n - 2*j;
        seq_t power = a000248_pow((seq_t)j, exponent);
        sum += j_fact * j_fact * power;
    }
    
    result = 2 * sum;
    
    if (n % 2 == 1) {
        idx_t half1 = (n + 1) / 2;
        seq_t half1_fact = j_fact * (seq_t)half1;
        seq_t half2_fact = j_fact;
        result += half1_fact * half2_fact;
    }
    
    return result;
}
