/*
 * A091257  Multiplication table A x B computed for polynomials over GF(2),
 *          where (A,B) runs as (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),...
 * https://oeis.org/A091257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   a(n) = product over GF(2) of the polynomials represented by
 *            integers A and B, where (A,B) is the nth pair in the
 *            anti‑diagonal enumeration: (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),
 * Range:     u8 n <= 480, u16 n <= 20100, u32 n <= 20100, u64 n <= 20100,
 *            u128 n <= 20100, big any n
 * Validated: standard, pass, 94 terms through n = 9828 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply two polynomials over GF(2) represented by integers a and b */
static seq_t mulGF2(seq_t a, seq_t b)
{
    seq_t res   = 0;
    seq_t acopy = a;
    idx_t  shift = 0;
    while (acopy)
    {
        if (acopy & 1)
            res ^= (b << shift);
        shift++;
        acopy >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        d++;

    /* previous diagonal count */
    idx_t prev = (d - 1) * d / 2;

    /* position within diagonal */
    idx_t i = n - prev;                /* 1 <= i <= d */
    seq_t A = (seq_t)i;
    seq_t B = (seq_t)(d + 1 - i);

    return mulGF2(A, B);
}
