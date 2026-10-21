/*
 * A072631  Floor( n*log(n) ) mod n.
 * https://oeis.org/A072631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*log(n)) mod n (log is natural logarithm)
 * Range:     u8 n <= 325, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for index 0, not used by OEIS offset */

    /* Compute n*ln(n) with long double for high precision */
    long double val = (long double)n * logl((long double)n);

    /* Initial floor estimate */
    unsigned long long floor_val = (unsigned long long)floorl(val + 1e-12L);

    /* Correct possible rounding errors */
    while ((long double)floor_val > val) floor_val--;
    while ((long double)(floor_val + 1) <= val) floor_val++;

    return (seq_t)(floor_val % (unsigned long long)n);
}
