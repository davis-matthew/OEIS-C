/*
 * A053995  Bases of n-dimensional vector space over GF(3).
 * https://oeis.org/A053995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^n-1)(3^n-3)(3^n-3^2)...(3^n-3^(n-1))/n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   _gcd_seq_t, _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the number of unordered bases of an n‑dimensional vector space
 * over GF(3).  The computation uses an iterative cancellation: at each
 * step we multiply by (3^n-3^k) and immediately divide by (k+1).  This
 * keeps the intermediate values small enough for the 64‑bit build and
 * works for arbitrary‑precision builds as well. */
#include <stdint.h>
#include "../utils/_pow_seq_t.h"            /* multiplicative power helper, if available */
#include "../utils/_gcd_seq_t.h"           /* gcd for possible cancellation, optional */

static seq_t pow3(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 0; i < n; ++i)
        r *= (seq_t)3;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t p3n = pow3(n);   /* 3^n */
    seq_t res = 1;
    seq_t p3k = 1;         /* 3^k , starts with 3^0 */

    for (idx_t k = 0; k < n; ++k) {
        seq_t factor = p3n - p3k;      /* 3^n - 3^k  */
        res *= factor;                /* multiply numerator */
        res /= (seq_t)(k + 1);        /* cancel part of n! */
        p3k *= (seq_t)3;              /* advance 3^k to next power */
    }
    return res;
}
