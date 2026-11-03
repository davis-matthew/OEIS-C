/*
 * A126779  Number of functions f:{1,2,...,n}->{1,2,...,n} such that Im(f)
 *          contains 3 fixed elements.
 * https://oeis.org/A126779
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - 3*(n-1)^n + 3*(n-2)^n - (n-3)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 381 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;  /* values below offset are irrelevant */

    /* Convert n to seq_t once */
    const seq_t sn   = (seq_t)n;
    const seq_t sn1  = sn - 1;            /* (n-1) */
    const seq_t sn2  = sn - 2;            /* (n-2) */
    const seq_t sn3  = sn - 3;            /* (n-3) */

    /* Compute the required powers */
    const seq_t pow_n  = ipow(sn,  sn);
    const seq_t pow_n1 = ipow(sn1, sn);
    const seq_t pow_n2 = ipow(sn2, sn);
    const seq_t pow_n3 = ipow(sn3, sn);

    /* Combine them according to the formula */
    seq_t res = pow_n;
    res -= (seq_t)3 * pow_n1;
    res += (seq_t)3 * pow_n2;
    res -= pow_n3;

    return res;
}
