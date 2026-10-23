/*
 * A023843  Sum of exponents in prime-power factorization of C(5n,n+1).
 * https://oeis.org/A023843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 665, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of exponents in prime‑power factorization of C(5n,n+1)
   (Legendre's theorem implementation). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* index error: return 0 for safety */

    idx_t limit = 5 * n;          /* upper bound for primes */
    size_t sz = (size_t)(limit + 1);

    /* Simple sieve of Eratosthenes to list all primes up to 'limit' */
    uint8_t *is_comp = (uint8_t *)calloc(sz, 1);   /* 0 = prime, 1 = composite */
    seq_t total = 0;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {           /* p is prime */
            idx_t power = p;
            seq_t exp = 0;

            while (power <= limit) {
                seq_t q1 = (seq_t)(limit / power);
                seq_t q2 = (seq_t)((n + 1) / power);
                seq_t q3 = (seq_t)((4 * n - 1) / power);
                exp += q1 - q2 - q3;

                if (power > limit / p) break;   /* avoid overflow */
                power *= p;
            }
            total += exp;

            if (p * p <= limit) {           /* mark multiples of p */
                for (idx_t m = p * p; m <= limit; m += p)
                    is_comp[m] = 1;
            }
        }
    }

    free(is_comp);
    return total;
}
