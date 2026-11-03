/*
 * A130208  Diagonalized matrix of A000203, sigma(n).
 * https://oeis.org/A130208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1815, u32 n <= 22155, u64 n <= 22155,
 *            u128 n <= 22155, big any n
 * Validated: standard, pass, 136 terms through n = 9328 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A130208 – Diagonalized matrix of sigma(n) (A000203)
 *
 *  a(n) =   sigma(k)   if  n = k*(k+1)/2  for some integer k ≥ 1
 *          0           otherwise
 *
 *  The function returns 0 for indices that are not triangular numbers
 *  and sigma(k) otherwise.  The computation uses the supplied
 *  integer square‑root routine `floor_sqrt` and the divisor‑sum
 *  routine `sigma_of`.  All arithmetic is performed in the
 *  sequence type `seq_t`, casting the index `n` to `seq_t` before
 *  multiplication, as required by the project specification.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* offset is 1; n=0 not used */
        return 0;

    /*  discriminant of k^2 + k - 2n = 0  is 1 + 8n  */
    seq_t tn          = (seq_t)n;
    seq_t discriminant = tn * 8 + 1;

    seq_t s = floor_sqrt(discriminant);   /* integer part of sqrt(1+8n) */
    if (s * s != discriminant)           /* not a perfect square → not triangular */
        return 0;

    seq_t k = (s - 1) / 2;                /* solve k = (-1 + sqrt(1+8n))/2 */
    if (k == 0)                           /* guard against invalid k */
        return 0;

    /*  k is within the range of idx_t for the anticipated n values  */
    return sigma_of((idx_t)k);
}
