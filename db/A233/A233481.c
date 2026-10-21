/*
 * A233481  Number of singletons (strong fixed points) in pair-partitions.
 * https://oeis.org/A233481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 0 * a(1) = 1 * a(n) = n * ( (3*n-4)*a(n-1)/(n-1) -
 *            (2*n-3)*a(n-2) ) for n > 1 * Recurrence uses exact integer
 *            division; (3*n-4)*a(n-1) is divisible by (n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;   /* a(k-2)  */
    seq_t a_nm1 = (seq_t)1;   /* a(k-1)  */

    seq_t a_curr;
    for (idx_t k = (idx_t)2; k <= n; ++k) {
        /* (3*k-4) fits in idx_t; cast to seq_t for multiplication */
        seq_t term1 = (seq_t)(3 * k - 4) * a_nm1;
        seq_t div   = term1 / (k - 1);                /* exact division */
        seq_t term2 = (seq_t)(2 * k - 3) * a_nm2;
        a_curr = (seq_t)k * (div - term2);

        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }
    return a_nm1;
}
