/*
 * A026320  sin(n) > cos(n) > sin(n+1).
 * https://oeis.org/A026320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) finds the n-th positive integer m such that 
 * sin(m) > cos(m) > sin(m+1).  
 * The function simply scans integers m = 1,2,3,... until the
 * n-th satisfying m is found and returns it as seq_t.
 */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;
    while (true) {
        double s  = sin((double)m);
        double c  = cos((double)m);
        double sn = sin((double)(m + 1));
        if (s > c && c > sn) {
            ++count;
            if (count == n) {
                return (seq_t)m;
            }
        }
        ++m;
    }
}
