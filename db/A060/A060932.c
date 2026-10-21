/*
 * A060932  Fifth convolution of Lucas numbers A000032(n+1), n >= 0.
 * https://oeis.org/A060932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*(125*n^5+825*n^4+1925*n^3+2895*n^2+2990*n+744)*L(n+2)
 *            + (1875*n^5+13125*n^4+31875*n^3+37875*n^2+29250*n+19200)*L(n+1))
 *            / 75000 L(k) is the k‑th Lucas number
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 18, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast doubling Fibonacci to obtain F(n), F(n+1) with seq_t arithmetic */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == (idx_t)0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a = (seq_t)0;   /* F(0) */
    seq_t b = (seq_t)1;   /* F(1) */

    int msb = 63 - __builtin_clzll(n);
    for (int i = msb; i >= 0; --i) {
        seq_t two_b          = b + b;
        seq_t two_b_minus_a  = two_b - a;
        seq_t c = a * two_b_minus_a;
        seq_t d = a * a + b * b;

        if ((n >> i) & 1) {
            a = d;
            b = c + d;
        } else {
            a = c;
            b = d;
        }
    }
    *fn  = a;
    *fn1 = b;
}

/* Lucas number L(n) = 2*F(n+1) - F(n) */
static seq_t lucas(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    seq_t two_fn1 = fn1 + fn1;
    return two_fn1 - fn;
}

seq_t a(idx_t n)
{
    /* Lucas numbers needed for every n */
    seq_t L1 = lucas(n + 1);  /* L(n+1) */
    seq_t L2 = lucas(n + 2);  /* L(n+2) */

    if (sizeof(seq_t) <= 8) {
        /* Small integer types: compute using 128‑bit intermediates */
        unsigned __int128 sn = n;

        unsigned __int128 p1 = 125;
        p1 = p1 * sn + 825;
        p1 = p1 * sn + 1925;
        p1 = p1 * sn + 2895;
        p1 = p1 * sn + 2990;
        p1 = p1 * sn + 744;

        unsigned __int128 p2 = 1875;
        p2 = p2 * sn + 13125;
        p2 = p2 * sn + 31875;
        p2 = p2 * sn + 37875;
        p2 = p2 * sn + 29250;
        p2 = p2 * sn + 19200;

        unsigned __int128 term1 = 25 * p1 * (unsigned __int128)L2;
        unsigned __int128 term2 = p2 * (unsigned __int128)L1;
        unsigned __int128 numerator = term1 + term2;

        return (seq_t)(numerator / 75000);
    } else {
        /* Arbitrary‑precision seq_t: use seq_t arithmetic only */
        seq_t sn = (seq_t)n;

        seq_t p1 = 125;
        p1 = p1 * sn + 825;
        p1 = p1 * sn + 1925;
        p1 = p1 * sn + 2895;
        p1 = p1 * sn + 2990;
        p1 = p1 * sn + 744;

        seq_t p2 = 1875;
        p2 = p2 * sn + 13125;
        p2 = p2 * sn + 31875;
        p2 = p2 * sn + 37875;
        p2 = p2 * sn + 29250;
        p2 = p2 * sn + 19200;

        seq_t numerator = 25 * p1 * L2 + p2 * L1;
        return numerator / 75000;
    }
}
