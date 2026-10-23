/*
 * A132370  Array read by antidiagonals: T(m,n) = number of spotlight tilings
 *          of a width 1 m X n frame.
 * https://oeis.org/A132370
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading T(m,n) = 2*(m-2)*(n-2)*(m+n-2) +
 *            (m-2)*(m+1) + (n-2)*(n+1) along antidiagonals starting at (3,3),
 *            where the antidiagonal sum s = m+n increases from 6 and the
 *            terms are enumerated with m increasing from 3 to s-3
 * Range:     u8 n <= 19, u16 n <= 1200, u32 n <= 1277, u64 n <= 1277,
 *            u128 n <= 1277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                      /* out of offset, value irrelevant */

    /* zero‑based position of the term in the diagonal listing */
    idx_t pos = n - 3;

    /* find k such that k*(k+1)/2 > pos.
       Here k = s - 5, where s is the antidiagonal sum. */
    idx_t k = 1;
    while (k * (k + 1) / 2 <= pos)
        ++k;

    /* antidiagonal sum s */
    idx_t s = k + 5;

    /* cumulative terms before this sum */
    idx_t cum_before = (k - 1) * k / 2;

    /* offset inside this sum (0‑based) */
    idx_t offset = pos - cum_before;

    /* compute m and n' for this term */
    idx_t m  = 3 + offset;
    idx_t n_ = s - m;                 /* n_ >= 3 */

    /* convert to seq_t for arithmetic */
    seq_t M = (seq_t)m;
    seq_t N = (seq_t)n_;

    /* T(m,n) = 2*(m-2)*(n-2)*(m+n-2) + (m-2)*(m+1) + (n-2)*(n+1) */
    seq_t term = 0;

    seq_t m2 = M - 2;
    seq_t n2 = N - 2;
    seq_t sum = M + N - 2;

    term += 2 * m2 * n2 * sum;
    term += m2 * (M + 1);
    term += n2 * (N + 1);

    return term;
}
