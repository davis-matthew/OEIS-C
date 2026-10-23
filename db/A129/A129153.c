/*
 * A129153  Rencontres numbers: permutations with exactly 8 fixed points.
 * https://oeis.org/A129153
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,8) * D(n-8) * where D(m) is the number of
 *            derangements of m elements: * D(0)=1, D(1)=0, D(m) =
 *            (m-1)*(D(m-1)+D(m-2)) for m>=2 * (Computed multiplicatively so
 *            no external binomial helper is needed.)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 17, u64 n <= 24, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)8)
        return (seq_t)0;          /* undefined below the offset */

    /* Compute binomial(n, 8) multiplicatively */
    const idx_t k = (idx_t)8;
    seq_t choose = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        choose *= (seq_t)(n - k + i);
        choose /= (seq_t)i;
    }

    /* Compute derangement D(n-8) by recurrence */
    idx_t m = n - (idx_t)8;
    seq_t d0 = (seq_t)1;  /* D(0) */
    if (m == (idx_t)0)
        return choose * d0;

    seq_t d1 = (seq_t)0;  /* D(1) */
    if (m == (idx_t)1)
        return choose * d1;

    seq_t dm = (seq_t)0;
    for (idx_t i = (idx_t)2; i <= m; ++i) {
        dm = (seq_t)(i - (idx_t)1) * (d1 + d0);
        d0   = d1;
        d1   = dm;
    }

    return choose * d1;
}
