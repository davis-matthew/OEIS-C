/*
 * A035213  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          31.
 * https://oeis.org/A035213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(31,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9820 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative computation using the Kronecker rule for m = 31. */

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

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* sequence defined for n≥1; safeguard */

    idx_t m = n;
    seq_t result = 1;

    /* Handle factor 2 */
    int e = 0;
    while ((m & 1ULL) == 0) { e++; m >>= 1ULL; }
    if (e > 0) result *= (seq_t)(e + 1);  /* 2 is in A038905 → factor e+1 */

    /* Handle odd prime factors up to sqrt(m) using p*p <= m */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) { exp++; m /= p; }

            if (p == 31) {
                /* 31 contributes a factor of 1 */
                continue;
            }

            int jac = jacobi_symbol(31ULL, p);  /* (31/p) */
            if (jac == 1) {
                result *= (seq_t)(exp + 1);      /* e+1 */
            } else if (jac == -1) {
                if (exp & 1) return 0;           /* odd exponent gives factor 0 */
                /* even exponent gives factor 1 → nothing to do */
            }
            /* jacobian cannot be 0 here because p ≠ 31 */
        }
    }

    /* Remaining prime factor > sqrt(original n) (if any) */
    if (m > 1) {
        if (m != 31) {
            int jac = jacobi_symbol(31ULL, m);
            if (jac == 1) {
                result *= (seq_t)2;              /* e = 1 → e+1 = 2 */
            } else if (jac == -1) {
                return 0;                       /* e = 1 odd → factor 0 */
            }
        }
        /* if m == 31, factor 1 → nothing to do */
    }

    return result;
}
