/*
 * A035177  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -13.
 * https://oeis.org/A035177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum_{d|n} Kronecker(-13,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the Jacobi symbol (a|n) where n is odd and n>0.
   Returns –1, 0, or 1. */
static int jacobi_symbol(uint64_t a, uint64_t n)
{
    int res = 1;
    a %= n;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t mod8 = n & 7;
            if (mod8 == 3 || mod8 == 5) res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3) res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Kronecker symbol (−13|d).  d is a positive integer. */
static int kronecker_neg13(idx_t d)
{
    if (d == 0) return 0;

    /* Factor out powers of two */
    int sign = 1;          /* cumulative sign for powers of two */
    idx_t t = d;
    while ((t & 1) == 0) {
        t >>= 1;
        sign = -sign;      /* each factor 2 contributes (−13|2)=−1 */
    }

    /* d is a power of two */
    if (t == 1) return sign;

    /* If 13 divides d, the symbol is zero */
    if (t % 13 == 0) return 0;

    /* Compute Jacobi symbol for the odd part */
    uint64_t a_mod = t - (13 % t);   /* (−13) mod t */
    int j = jacobi_symbol(a_mod, t);
    return sign * j;
}

/* Main function that returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    int64_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += kronecker_neg13(d);
            idx_t other = n / d;
            if (other != d) sum += kronecker_neg13(other);
        }
    }
    return (seq_t)sum;
}
