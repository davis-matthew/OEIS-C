/*
 * A055430  Number of points in Z^n of norm <= 6.
 * https://oeis.org/A055430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 31, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function computes a(n) = the number of integer points (x_1,…,x_n) in ℤ^n
   such that x_1²+…+x_n² ≤ 36 (the Euclidean ball of radius 6).
   The computation uses the generating function
          F(z) = 1 + 2(z + z^4 + z^9 + z^16 + z^25 + z^36),
   and a(n) is the sum of the coefficients of F(z)^n up to degree 36.
   Exponentiation is performed by binary exponentiation with
   polynomials truncated to degree 36.  Each polynomial is stored
   as an array of 37 coefficients of type seq_t.
*/

#include <stddef.h>

static const int MAX_DEG = 36;

/* Multiply two polynomials of degree ≤ 36, truncating to degree 36.
   a[0..36], b[0..36] are input coefficients; res[0..36] receives the product. */
static void poly_mult(const seq_t *a, const seq_t *b, seq_t *res)
{
    seq_t tmp[37] = {0};
    for (idx_t i = 0; i <= MAX_DEG; ++i) {
        if (a[i] == 0) continue;
        for (idx_t j = 0; j + i <= MAX_DEG; ++j) {
            if (b[j] == 0) continue;
            tmp[i + j] += a[i] * b[j];
        }
    }
    for (idx_t k = 0; k <= MAX_DEG; ++k) res[k] = tmp[k];
}

/* Compute F(z)^n, where F is the fixed polynomial */
static seq_t poly_pow(idx_t n)
{
    /* Initial polynomial F(z) */
    seq_t base[37] = {0};
    base[0] = 1;
    base[1] = 2;
    base[4] = 2;
    base[9] = 2;
    base[16] = 2;
    base[25] = 2;
    base[36] = 2;

    /* Result polynomial initialized to F(z)^0 = 1 */
    seq_t res[37] = {0};
    res[0] = 1;

    while (n) {
        if (n & 1) {
            seq_t tmp[37];
            poly_mult(res, base, tmp);
            for (idx_t k = 0; k <= MAX_DEG; ++k) res[k] = tmp[k];
        }
        n >>= 1;
        if (n) {
            seq_t tmp[37];
            poly_mult(base, base, tmp);
            for (idx_t k = 0; k <= MAX_DEG; ++k) base[k] = tmp[k];
        }
    }

    /* Sum coefficients up to degree 36 */
    seq_t total = 0;
    for (idx_t k = 0; k <= MAX_DEG; ++k) total += res[k];
    return total;
}

/* Public interface */
seq_t a(idx_t n)
{
    return poly_pow(n);
}
