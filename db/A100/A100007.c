/*
 * A100007  Number of unitary divisors of 2n-1 (d such that d divides 2n-1,
 *          GCD(d,(2n-1)/d)=1). Bisection of A034444.
 * https://oeis.org/A100007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^omega(2*n-1) (unitary divisor count of 2*n-1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* offset starts at 1; for n==0 the result is undefined, return 0 */
    if (n == 0) return (seq_t)0;

    /* compute m = 2*n - 1 using seq_t arithmetic */
    seq_t m = ((seq_t)2 * (seq_t)n) - (seq_t)1;
    seq_t temp = m;

    /* count distinct prime factors of m */
    unsigned int distinct = 0;
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p) {
        if (temp % (seq_t)p == 0) {
            ++distinct;
            while (temp % (seq_t)p == 0)
                temp /= (seq_t)p;
        }
    }
    if (temp > (seq_t)1)
        ++distinct;

    /* return 2^distinct using shift with an integral shift amount */
    return (seq_t)1 << distinct;
}
