/*
 * A094527  Triangle T(n,k), read by rows, defined by T(n,k) =
 *          binomial(2*n,n-k).
 * https://oeis.org/A094527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(2*r, r-k) where r = floor((sqrt(8*n+1)-1)/2) and
 *            k = n - r*(r+1)/2
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 170, u64 n <= 594,
 *            u128 n <= 2210, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Use seq_t for all intermediate calculations to avoid overflow. */
    seq_t nseq = (seq_t)n;

    /* Compute s = floor_sqrt(8*n + 1). */
    seq_t x = ((seq_t)8 * nseq) + (seq_t)1;
    seq_t s = floor_sqrt(x);

    /* r = (s - 1) / 2 */
    seq_t rseq = (s - (seq_t)1) / (seq_t)2;

    /* prefix = r*(r+1)/2 */
    seq_t prefix = rseq * (rseq + (seq_t)1) / (seq_t)2;

    /* k = n - prefix */
    seq_t kseq = nseq - prefix;

    /* Convert to idx_t for the binomial arguments. */
    idx_t r = (idx_t)rseq;
    idx_t k = (idx_t)kseq;

    idx_t twor = r << 1;      /* 2 * r */
    idx_t rk   = r - k;       /* r - k */

    return binomial(twor, rk);
}
