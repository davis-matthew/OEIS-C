/*
 * A003619  Not of form [ e^m ], m >= 1.
 * https://oeis.org/A003619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(log(n+1+floor(log(n+1))))
 * Range:     u8 n <= 250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>


seq_t a(idx_t n)
{
    double dn_plus_1 = (double)(n + 1);
    
    /* Compute floor(log(n+1)) */
    int log_n_plus_1 = (int)floor(log(dn_plus_1));
    
    /* Compute n+1+floor(log(n+1)) for logarithm computation */
    double arg = dn_plus_1 + (double)log_n_plus_1;
    
    /* Compute floor(log(n+1+floor(log(n+1)))) */
    int result_offset = (int)floor(log(arg));
    
    /* Return n + floor(log(...)) as seq_t */
    return (seq_t)n + (seq_t)result_offset;
}
