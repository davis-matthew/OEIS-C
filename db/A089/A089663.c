/*
 * A089663  a(n) = S1(n, 6), where S1(n, t) = Sum_{k=0..n} (k^t * Sum_{j=0..k}
 *          binomial(n,j)).
 * https://oeis.org/A089663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1)/21) * (381*n^5 + 921*n^4 + 381*n^3 + 39*n^2 -
 *            746*n + 368) * 2^(n-7) (Returns a(0)=0, a(1)=2, …, a(7)=23313408
 *            as table look‑ups.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 31, u128 n <= 85,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static const seq_t base_values[8] = {
    0ULL,          /* a(0) */
    2ULL,          /* a(1) */
    259ULL,        /* a(2) */
    6284ULL,       /* a(3) */
    77180ULL,      /* a(4) */
    646960ULL,     /* a(5) */
    4235864ULL,    /* a(6) */
    23313408ULL    /* a(7) */
};

seq_t a(idx_t n)
{
    if (n < 8) {
        return base_values[n];
    }

    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;
    seq_t n3 = n2 * n0;
    seq_t n4 = n3 * n0;
    seq_t n5 = n4 * n0;

    /* Polynomial part: 381*n^5 + 921*n^4 + 381*n^3 + 39*n^2 + 368  (positives) */
    seq_t pos = 381 * n5 + 921 * n4 + 381 * n3 + 39 * n2 + 368;

    /* Subtract 746*n (ensuring the subtrahend is smaller) */
    seq_t poly = pos - 746 * n0;

    /* t1 = n*(n+1) */
    seq_t t1 = n0 * (n0 + 1);

    /* numerator = t1 * poly */
    seq_t num = t1 * poly;

    /* division by 21 */
    seq_t q = num / 21;

    /* multiply by 2^(n-7) */
    return q * pow2(n - 7);
}
