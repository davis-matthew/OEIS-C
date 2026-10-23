/*
 * A140666  A triangle of coefficients of the difference of prime cyclotomic
 *          doubled polynomials: p(x,n)=(c(x,Prime[n])-c(x,2*Prime[n]))/x.
 * https://oeis.org/A140666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 111, i16 n <= 111, i32 n <= 111, i64 n <= 111,
 *            i128 n <= 111, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140666: Flattened coefficients of (Φ_p(x)-Φ_{2p}(x))/x for primes p=2,3,5,...
   Each row corresponds to a prime p.  
   For p=2 the row is {1,-1}.  
   For odd p the row has length p-2, with coefficient 2 at even positions
   (exponent 0,2,4,…,p-3) and 0 at odd positions. */

/* Include the helper function that recognises primes. */
#include "../utils/is_prime.h"
#include <stdbool.h>

/* Return the n-th term of the flattened triangle (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* below offset, behaviour not required */

    idx_t remaining = n;          /* how many positions left to skip */
    idx_t candidate = 2;           /* candidate for next prime */

    while (1) {
        if (is_prime(candidate)) {
            /* length of the current row */
            idx_t len = (candidate == 2) ? 2 : candidate - 2;

            if (remaining <= len) {
                /* Found the required row and position. */
                if (candidate == 2) {
                    /* Row for prime 2: {1, -1} */
                    return (remaining == 1) ? (seq_t)1 : (seq_t)-1;
                } else {
                    /* Row for odd prime: coefficient 2 at even exponents. */
                    idx_t exp = remaining - 1;  /* exponent (0‑based) */
                    return ((exp & 1) == 0) ? (seq_t)2 : (seq_t)0;
                }
            } else {
                /* Skip this whole row and continue. */
                remaining -= len;
            }
        }
        /* Advance to the next integer. */
        candidate++;
    }
}
