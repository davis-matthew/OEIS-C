/*
 * A086823  Coefficient of x^2 in the n-th cyclotomic polynomial. (The same as
 *          the coefficient of x^(phi(n)-2) ).
 * https://oeis.org/A086823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/2)·μ(n)·(μ(n)−1) if n is odd, * a(n) =
 *            (1/2)·μ(n)·(μ(n)−1) − μ(n/2) if n is even * where μ is the
 *            Möbius function
 * Range:     i8 n <= 9639, i16 n <= 9639, i32 n <= 9639, i64 n <= 9639,
 *            i128 n <= 9639, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the coefficient of x² in the n‑th cyclotomic polynomial,
 *  a(n) is defined for n ≥ 1.  */

#include <stdint.h>

/*  Compute the Möbius μ(n).  Returns 0, 1 or −1.  */
static int mu_func(idx_t n)
{
    if (n == 1) return 1;
    int count = 0;
    /* handle factor 2 */
    if ((n & 1ULL) == 0) {
        count++;
        n >>= 1;
        if ((n & 1ULL) == 0) return 0;   /* squared factor */
    }
    /* odd factors */
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            count++;
            n /= p;
            if (n % p == 0) return 0;   /* squared factor */
        }
    }
    if (n > 1) count++;
    return (count & 1) ? -1 : 1;
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    int mu_n   = mu_func(n);
    seq_t  r   = (seq_t)mu_n * ((seq_t)mu_n - 1);  /* μ(n)·(μ(n)–1)  */
    r /= 2;                                         /* divide by 2      */
    if ((n & 1UL) == 0) {                           /* n even          */
        r -= (seq_t)mu_func(n / 2);                  /* subtract μ(n/2) */
    }
    return r;
}
