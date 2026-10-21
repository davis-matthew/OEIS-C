/*
 * A202990  E.g.f: Sum_{n>=0} 3^n * 2^(n^2) * exp(-2*2^n*x) * x^n/n!.
 * https://oeis.org/A202990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*2^n - 2)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static seq_t pow_seq_local(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t two_pow = pow_seq_local((seq_t)2, n);      /* 2^n */
    seq_t base    = ((seq_t)3 * two_pow) - ((seq_t)2); /* 3*2^n - 2 */
    return pow_seq_local(base, n);                    /* (3*2^n-2)^n */
}
