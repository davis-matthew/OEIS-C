/*
 * A004529  Ratios of successive terms are 1,1,1,2,3,3,3,4,5,5,5,6,...
 * https://oeis.org/A004529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(4*k+j) = 2^k * k! * (2k-1)!!^3 * (2*k+1)^j for 0 <= j <= 3
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 41,
 *            big any n
 * Helpers:   A001832_pow2, a000248_pow, double_factorial_odd, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"
#include "../utils/a000248_pow.h"
#include "../utils/double_factorial_odd.h"
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    idx_t k = n / 4;
    idx_t j = n % 4;
    
    seq_t pow2k = A001832_pow2(k);
    seq_t kfact = factorial(k);
    seq_t double_fact = double_factorial_odd(k);
    seq_t double_fact_cubed = double_fact * double_fact * double_fact;
    seq_t term2k1_pow_j = a000248_pow((seq_t)(2 * k + 1), j);
    
    return pow2k * kfact * double_fact_cubed * term2k1_pow_j;
}
