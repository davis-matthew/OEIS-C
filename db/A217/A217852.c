/*
 * A217852  Multiplicative order of 5 (mod 5*n - 1).
 * https://oeis.org/A217852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 5 modulo (5*n - 1)
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset not used, but safe */
    /* Modulus M = 5*n - 1 ; cast n to seq_t before multiplication to avoid overflow */
    seq_t mod = ((seq_t)n * 5) - 1;
    seq_t pow = 1 % mod;            /* initial value 5^0 mod M */

    /* Find smallest m >= 1 with 5^m ≡ 1 (mod M) */
    for (seq_t m = 1; ; ++m)
    {
        pow = (pow * 5) % mod;      /* compute 5^m mod M iteratively */
        if (pow == 1) return m;
    }
}
