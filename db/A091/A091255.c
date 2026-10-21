/*
 * A091255  Square array computed from gcd(P(x),P(y)) where P(x) and P(y) are
 *          polynomials with coefficients in {0,1} given by the binary
 *          expansions of x and y, and the polynomial calculation is done over
 *          GF(2), with the result converted back to a binary number, and then
 *          expressed in decimal. Array is symmetr...
 * https://oeis.org/A091255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = A091255 computed by taking the antidiagonal index (i,j)
 *            of n in the symmetric array A(x,y) = gcd_GF2(x,y) and returning
 *            the gcd of the polynomials represented by the binary expansions
 *            of i and j over GF(2)
 * Range:     u8 n <= 9980, u16 n <= 9980, u32 n <= 9980, u64 n <= 9980,
 *            u128 n <= 9980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute the degree of a polynomial represented by an unsigned integer.
   The degree is the index of the highest set bit.  Zero has degree 0. */
static unsigned int poly_degree(seq_t p)
{
    if (p == 0)
        return 0;
    unsigned int d = 0;
    while (p)
    {
        p >>= 1;
        d++;
    }
    return d - 1;
}

/* Compute a mod b for polynomials over GF(2).
   a and b are treated as binary polynomial coefficients. */
static seq_t poly_mod(seq_t a, seq_t b)
{
    if (b == 0)
        return a;                     /* not expected for the array */
    unsigned int deg_b = poly_degree(b);
    while (a != 0)
    {
        unsigned int deg_a = poly_degree(a);
        if (deg_a < deg_b)
            break;
        unsigned int shift = deg_a - deg_b;
        a ^= (b << shift);
    }
    return a;
}

/* Greatest common divisor of two polynomials over GF(2). */
static seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* Convert linear index n (starting at 1) to antidiagonal coordinates (i,j). */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t D = 1;
    idx_t sum = 1;
    while (sum < n)
    {
        ++D;
        sum += D;
    }
    idx_t prev_sum = sum - D;
    idx_t offset = n - prev_sum - 1;   /* zero based offset on this diagonal */
    *i = offset + 1;
    *j = D - offset;
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  The offset is 1, so n>=1 is guaranteed. */
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return poly_gcd((seq_t)i, (seq_t)j);
}
