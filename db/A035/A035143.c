/*
 * A035143  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -47.
 * https://oeis.org/A035143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-47, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* Handle factor 2 separately.  Kronecker(-47, 2) = 1. */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            ++e;
            m >>= 1;
        }
        result = result * (seq_t)(e + 1);
    }

    /* Trial division for odd factors. */
    for (uint64_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            uint64_t e = 0;
            while (m % i == 0) {
                ++e;
                m /= i;
            }

            if (i == 47) {
                /* For p = 47, the contribution is 1 for every exponent. */
            } else {
                uint64_t a_val = i - (47 % i);   /* (-47) mod i, positive */
                int t = jacobi_symbol(a_val, i); /* Kronecker(-47, i) */

                if (t == 1) {
                    result = result * (seq_t)(e + 1);
                } else if (t == -1) {
                    if (e & 1ULL) {
                        result = 0;
                    }
                    /* If e is even, multiply by 1: no change. */
                }
            }

            if (result == 0) {
                break; /* product will stay 0 */
            }
        }
    }

    /* Remaining prime factor, if any. */
    if (result != 0 && m > 1) {
        uint64_t p = m;
        if (p == 47) {
            /* contribution stays 1 */
        } else {
            uint64_t a_val = p - (47 % p);
            int t = jacobi_symbol(a_val, p);

            if (t == 1) {            /* e = 1 */
                result = result * (seq_t)(2); /* 1 + 1 */
            } else if (t == -1) {    /* e = 1 odd -> 0 */
                result = 0;
            }
        }
    }

    return result;
}
