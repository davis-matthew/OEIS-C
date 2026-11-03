/*
 * A071055  Number of 0's in n-th row of triangle in A071038.
 * https://oeis.org/A071055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n+1) with b(0) = 0, b(2k) = b(k), b(2k+1) = 2*b(k)+2
 *            (with the additional subtraction of 2 when k = 0, i.e. b(1) = 0)
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9033 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t compute_b(idx_t k)
{
    if (k <= 1)                           /* b(0) = b(1) = 0 */
        return 0;

    if ((k & 1ULL) == 0)                  /* even k: b(k) = b(k/2) */
        return compute_b(k >> 1);

    /* odd k > 1: b(k) = 2*b(k/2) + 2 */
    seq_t res = compute_b(k >> 1);
    res = res * ((seq_t)2);
    res += (seq_t)2;
    return res;
}

seq_t a(idx_t n)
{
    return compute_b(n + 1);
}
