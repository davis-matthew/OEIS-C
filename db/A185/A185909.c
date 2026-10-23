/*
 * A185909  Accumulation array of A185908, by antidiagonals.
 * https://oeis.org/A185909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value at (i,j) of the accumulation array A185908 where
 *            (i,j) is the n‑th element in antidiagonal order. The
 *            accumulation array is defined by the weight array: w(p,q) = {
 *            p+q-1 if q ≤ p 2p-1 if q > p } and A185909(i,j) = Σ_{p=1..i}
 *            Σ_{q=1..j} w(p,q)
 * Range:     u8 n <= 84, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Note: All arithmetic that fits into the target value type seq_t
    is performed using the wider idx_t type to avoid overflow.   */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* zero is not defined, but guard just in case */
    if (n == 0) return 0;

    /* find diagonal k (1‑based) such that k*(k+1)/2 >= n   */
    idx_t k = 1;
    idx_t tri = 1;                 /* triangular number t(k) = k*(k+1)/2 */
    while (tri < n) {
        ++k;
        tri += k;
    }

    /* coordinates (i,j) of the n-th element, 1‑based            */
    idx_t prev_tri = tri - k;          /* t(k-1) */
    idx_t offset   = n - prev_tri - 1; /* 0‑based within diagonal */
    idx_t i = offset + 1;
    idx_t j = k - offset;              /* because i + j - 1 = k */

    /* compute accumulation value using formulas derived above   */
    seq_t result;
    if (i <= j) {
        /* A = i*(i+1)*(6*j - i + 1)/6 - i*j                     */
        idx_t t1 = i * (i + 1);
        idx_t t2 = 6 * j - i + 1;
        idx_t mult = t1 * t2;
        idx_t div = mult / 6;
        result = (seq_t)div - (seq_t)i * (seq_t)j;
    } else {
        /* A = j*(5*j*j + 1)/6 + ( j*(i*(i+1) - j*(j+1)) + (i-j)*j*(j-1) )/2   */
        idx_t val1 = j * ((5 * j * j) + 1);
        val1 /= 6;
        idx_t termA = j * ((i * (i + 1)) - (j * (j + 1)));
        idx_t termB = (i - j) * j * (j - 1);
        idx_t term = (termA + termB) / 2;
        result = (seq_t)val1 + (seq_t)term;
    }

    return result;
}
