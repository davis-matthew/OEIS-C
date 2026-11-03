/*
 * A188588  Row sums of 1-Euler triangle A188587.
 * https://oeis.org/A188588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*a(n-1) for n>=3; a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = (n+1)!/2 for n>=2 */

#include <stdlib.h>

static seq_t compute_direct(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    seq_t val = (seq_t)3;               /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        val = ((seq_t)(i + 1)) * val;   /* a(i) = (i+1)*a(i-1) */
    }
    return val;
}

seq_t a(idx_t n)
{
    return compute_direct(n);
}
