/*
 * A079946  Numbers k whose binary expansion begins with two or more 1's and
 *          ends with at least one 0.
 * https://oeis.org/A079946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2^floor(log_2(4*n)) + 2*n This is the equivalent of the
 *            recurrence: a(2n) = 2*a(n) a(2n+1) = 2*a(n) + 2 + 4[n==0] (n>=0)
 *            for a(1) = 6, a(2) = 12,
 * Range:     u8 n <= 63, u16 n <= 9632, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be >= 1 (offset 1) */
    idx_t t = 4 * n;                         /* 4*n, always non‑zero for n>=1 */

    /* Compute floor(log2(t)).  Since t >= 4, the result fits in an int. */
    unsigned int p = 0;
    /* Use a small loop; t <= 4*UINT64_MAX fits in 64 bits. */
    while ((t >> (p + 1)) != 0) {
        p++;
    }

    /* Compute 2^p in the result type */
    seq_t two_pow = ((seq_t)1) << (int)p;    /* shift count cast to int */

    /* Return the formula value */
    return two_pow + (seq_t)(2 * n);
}
