/*
 * A105188  a(n) = determinant of the n X n matrix m(i,j)=(i+j+3)!/i!/j!.
 * https://oeis.org/A105188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/288)*(n+1)*(n+2)!*(n+3)!*(n+4)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for unsigned 64‑bit values */
static unsigned long long gcd64(unsigned long long a, unsigned long long b)
{
    while (b != 0) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Multiply *res by factor, then cancel any common factor with the running
   denominator.  The denominator starts at 288 and is fully cancelled by
   the end of the calculation. */
static void mul_div_gcd(seq_t *res, unsigned long long factor,
                        unsigned long long *denom)
{
    unsigned long long g = gcd64(factor, *denom);
    factor   /= g;
    *denom   /= g;
    *res     *= (seq_t)factor;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    unsigned long long denom = 288ULL;          /* 2^5 * 3^2 */

    /* multiply by (n+1) */
    mul_div_gcd(&result, (unsigned long long)(n + 1), &denom);

    /* multiply by (n+4) */
    mul_div_gcd(&result, (unsigned long long)(n + 4), &denom);

    /* multiply by (n+3) twice (for (n+3)!^2) */
    unsigned long long n3 = (unsigned long long)(n + 3);
    mul_div_gcd(&result, n3, &denom);
    mul_div_gcd(&result, n3, &denom);

    /* multiply by (n+2)! three times (for (n+2)!^3) */
    idx_t limit = n + 2;
    for (idx_t k = 2; k <= limit; ++k) {
        mul_div_gcd(&result, (unsigned long long)k, &denom);
        mul_div_gcd(&result, (unsigned long long)k, &denom);
        mul_div_gcd(&result, (unsigned long long)k, &denom);
    }

    /* any remaining denominator should be 1; if not, divide it out */
    if (denom != 1) {
        result /= (seq_t)denom;
    }

    return result;
}
