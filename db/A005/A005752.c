/*
 * A005752  a(n) = n^2 + n*floor(n*tau) - floor(n*tau)^2.
 * https://oeis.org/A005752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + n*floor(n*tau) - floor(n*tau)^2, where tau =
 *            (1+sqrt(5))/2
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
        
    double tau = (1.0 + sqrt(5.0)) / 2.0;
    idx_t floor_n_tau = (idx_t)floor((double)n * tau);
    
    seq_t n_s = (seq_t)n;
    seq_t fn_s = (seq_t)floor_n_tau;
    
    return n_s * n_s + n_s * fn_s - fn_s * fn_s;
}
