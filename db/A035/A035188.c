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
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative implementation using prime factorisation and the Jacobi symbol */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

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
