/*
 * A131189  Numbers n >= 0 such that d(n) = (n^1 + 1)*(n^2 + 2) ... (n^14 +
 *          14) / 14!, e(n) = (n^1 + 1)*(n^2 + 2) ... (n^15 + 15) / 15!, and
 *          f(n) = (n^1 + 1)*(n^2 + 2) ... (n^16 + 16) / 16! take nonintegral
 *          values.
 * https://oeis.org/A131189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((7*(n-1)+1)/6) + 2 if (n-1) % 6 != 5, * a(n) =
 *            7*floor((7*(n-1)+1)/6) - 5 if (n-1) % 6 == 5. * (This yields
 *            a(1)=2, a(2)=9, a(3)=16, … as in OEIS A131189.)
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n-1  (n>=1) */
    idx_t m = n - 1;
    /* t = floor((7*m + 1) / 6) */
    idx_t t = ((idx_t)7 * m + 1) / 6;
    /* base = 7 * t */
    seq_t base = (seq_t)7 * (seq_t)t;

    if (m % 6 == 5)
        return base - 5;
    else
        return base + 2;
}
