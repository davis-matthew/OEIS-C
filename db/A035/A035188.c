/*
 * A035188  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          6.
 * https://oeis.org/A035188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(6,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9852 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative implementation using prime factorisation and the Jacobi symbol */

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
    /* Handle n == 0 which is out of range; return 0 */
    if (n == 0) return 0;

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;
    uint64_t exp;
    uint64_t p;

    /* Factor out 2 */
    while ((temp & 1ull) == 0) {
        temp >>= 1;           /* divide by 2 */
    }
    /* 2 divides 6, so Kronecker(6,2) = 0 and a(2^e) = 1 => no change to result */

    /* Factor out 3 */
    while (temp % 3ull == 0) {
        temp /= 3ull;
    }
    /* 3 divides 6, so Kronecker(6,3) = 0 and a(3^e) = 1 => no change */

    /* Process remaining odd primes */
    for (p = 5ull; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            int k = jacobi_symbol(6ull, p); /* -1, 1 */
            seq_t local;
            if (k == -1) {
                /* (1 + (-1)^e) / 2  -> 1 if e even, 0 if e odd */
                local = (exp & 1ull) ? 0 : 1;
            } else { /* k == 1 */
                local = (seq_t)(exp + 1);   /* e+1 */
            }
            result *= local;
        }
    }

    /* If a prime factor remains */
    if (temp > 1) {
        exp = 1;
        int k = jacobi_symbol(6ull, temp);
        seq_t local;
        if (k == -1) {
            local = 0; /* exp is odd */
        } else { /* k == 1 */
            local = (seq_t)(exp + 1);   /* 2 */
        }
        result *= local;
    }

    return result;
}
