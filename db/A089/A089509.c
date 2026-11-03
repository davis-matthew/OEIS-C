/*
 * A089509  a(n) = Kronecker(7/n).
 * https://oeis.org/A089509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Kronecker(7/n) (Kronecker symbol with numerator 7)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 122 terms through n = 989 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute Jacobi symbol (a|n) for odd n > 0.  Assumes gcd(a,n)=1.
 * The algorithm is the standard binary algorithm based on
 * quadratic reciprocity.  It returns -1 or +1. */
static int jacobi_prime7(int64_t a, int64_t n)
{
    a %= n;
    int sign = 1;

    while (a != 0) {
        /* Handle factors of 2 in a */
        while ((a & 1) == 0) {
            a >>= 1;
            int n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                sign = -sign;
        }

        /* Swap a and n, apply quadratic reciprocity */
        int64_t temp = a;
        a = n;
        n = temp;

        if ((a & 3) == 3 && (n & 3) == 3)
            sign = -sign;

        a %= n;
    }

    return (n == 1) ? sign : 0;   /* n==1 => result; else gcd!=1 -> 0 */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* Not defined for offset, return 0 */

    /* If 7 divides n the Kronecker symbol is 0. */
    if (n % 7 == 0)
        return (seq_t)0;

    /* Strip powers of 2: (7|2)=1, so they don't affect the value. */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;                  /* m is now odd */

    int j = jacobi_prime7(7, (int64_t)m);  /* m fits in int64_t */
    return (seq_t)j;          /* j is ±1 */
}
