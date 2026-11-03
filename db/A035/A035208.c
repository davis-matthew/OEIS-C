/*
 * A035208  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          26.
 * https://oeis.org/A035208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(26,d) (multiplicative form: a(p^e) =
 *            1 if p∈{2,13}, = (1+(-1)^e)/2 if Kronecker(26,p) = -1, = e+1 if
 *            Kronecker(26,p) = 1 ) Implemented by factorising n and
 *            multiplying the local contributions for each prime power
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9900 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* if n==0 we return 0 – the offset is 1 but the behaviour below is irrelevant */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 1;
    idx_t m = n;

    /* Remove the factor 2 – its contribution is always 1  */
    if ((m & 1) == 0) {
        while ((m & 1) == 0)
            m >>= 1;
    }

    /* Trial division for odd primes.  Use the simple bound p <= m/p
       which automatically adjusts to the current value of m after each
       division.  */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            /* count exponent of p */
            while (m % p == 0) {
                m /= p;
                e++;
            }

            int k = jacobi_symbol(26ULL, p);  /* Kronecker(26,p)  */
            seq_t contrib;
            if (k == 0)          /* p == 13  */
                contrib = 1;
            else if (k == 1)     /* Kronecker = +1  */
                contrib = (seq_t)(e + 1);
            else                 /* Kronecker = -1  */
                contrib = ((e & 1) == 0) ? (seq_t)1 : (seq_t)0;

            result *= contrib;
            if (result == 0)
                return 0;       /* multiplied by 0 stays 0, short‑circuit */
        }
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (m > 1) {
        int k = jacobi_symbol(26ULL, m);   /* m is odd prime */
        seq_t contrib;
        if (k == 0)          /* m == 13  */
            contrib = 1;
        else if (k == 1)     /* Kronecker = +1  */
            contrib = 2;     /* e+1 with e=1 */
        else                 /* Kronecker = -1  */
            contrib = 0;     /* (1+(-1)^1)/2 = 0 */
        result *= contrib;
    }

    return result;
}
