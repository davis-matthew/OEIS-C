/*
 * A034127  Decimal part of cube root of a(n) starts with 1: first term of
 *          runs.
 * https://oeis.org/A034127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil((10*n+11)^3 / 1000)
 * Range:     u8 none, u16 n <= 2, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t k  = nn * 10 + 11;          /* 10*n + 11 */
    seq_t k3 = k * k * k;             /* (10*n+11)^3 */
    seq_t res = (k3 + 999) / 1000;    /* ceil division by 1000 */
    return res;
}
