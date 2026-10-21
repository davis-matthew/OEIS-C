/*
 * A070923  a(n) is the smallest value >= 0 of the form x^3 - n^2.
 * https://oeis.org/A070923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, look, nonn
 * Formula:   a(n) = ceil(n^(2/3))^3 - n^2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 6836, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cuberoot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest non‑negative value of the form x^3 - n^2. */
#include "../utils/floor_cuberoot.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;                 /* n^2 */
    seq_t k   = floor_cuberoot(n2);         /* floor of cube root of n^2 */
    seq_t k2  = k * k;
    seq_t k3  = k2 * k;

    /* if k^3 < n^2, we need the next integer cube root */
    if (k3 < n2)
        k++;

    seq_t kk3 = k * k * k;                 /* ceil(n^(2/3))^3 */
    return kk3 - n2;                       /* smallest non‑negative x^3 - n^2 */
}
