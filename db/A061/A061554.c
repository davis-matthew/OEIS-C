/*
 * A061554  Square table read by antidiagonals: a(n,k) = binomial(n+k,
 *          floor(k/2)).
 * https://oeis.org/A061554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(d, floor((d - j)/2)) where d = smallest integer
 *            with (d+1)(d+2)/2 > n and j = n - d(d+1)/2 (the offset within
 *            diagonal d). The triangle element is a(n,k)=binomial(n+k,
 *            floor(k/2)), and on diagonal d we have n+k = d
 * Range:     u8 n <= 35, u16 n <= 189, u32 n <= 560, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d that contains the n-th term. */
    idx_t d = 0;            /* current diagonal index   */
    idx_t prev = 0;         /* cumulative count up to d-1 */

    while (1)
    {
        idx_t seg = d + 1;                                  /* size of diagonal d   */
        unsigned __int128 next = (unsigned __int128)prev + seg;
        if (next > n) break;                               /* this diagonal is the one */
        prev = (idx_t)next;                                 /* advance to next diagonal */
        ++d;
    }

    /* offset within diagonal d */
    idx_t j = n - prev;
    /* corresponding k value on this diagonal */
    idx_t k = d - j;
    /* floor(k/2) */
    idx_t m = k / 2;

    return binomial(d, m);
}
