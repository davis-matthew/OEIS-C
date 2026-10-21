/*
 * A207816  Number of distinct necklaces with n red, n green, n blue and n
 *          white beads.
 * https://oeis.org/A207816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(4*n)) * Sum_{d|n} phi(n/d) * (4*d)!/(d!^4) (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/phi.h"       /* seq_t phi(idx_t m); */

seq_t a(idx_t n)
{
    /* special case */
    if (n == 0)  return (seq_t)1;

    /* for built‑in small integer types use 128‑bit arithmetic to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;
        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                /* divisor i */
                {
                    idx_t d = i;
                    seq_t phi_val = phi(n / d);
                    seq_t b1 = binomial(4 * d, d);
                    seq_t b2 = binomial(3 * d, d);
                    seq_t b3 = binomial(2 * d, d);

                    unsigned __int128 prod = (unsigned __int128)phi_val;
                    prod *= (unsigned __int128)b1;
                    prod *= (unsigned __int128)b2;
                    prod *= (unsigned __int128)b3;
                    sum += prod;
                }
                /* paired divisor n/i, if distinct */
                if (i != n / i) {
                    idx_t d = n / i;
                    seq_t phi_val = phi(n / d);
                    seq_t b1 = binomial(4 * d, d);
                    seq_t b2 = binomial(3 * d, d);
                    seq_t b3 = binomial(2 * d, d);

                    unsigned __int128 prod = (unsigned __int128)phi_val;
                    prod *= (unsigned __int128)b1;
                    prod *= (unsigned __int128)b2;
                    prod *= (unsigned __int128)b3;
                    sum += prod;
                }
            }
        }
        unsigned __int128 denom = (unsigned __int128)4 * n;
        return (seq_t)(sum / denom);
    }

    /* otherwise use seq_t arithmetic (works for arbitrary‑precision types) */
    else {
        seq_t sum = 0;
        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                /* divisor i */
                {
                    idx_t d = i;
                    seq_t prod = phi(n / d);
                    prod *= binomial(4 * d, d);
                    prod *= binomial(3 * d, d);
                    prod *= binomial(2 * d, d);
                    sum += prod;
                }
                /* paired divisor n/i, if distinct */
                if (i != n / i) {
                    idx_t d = n / i;
                    seq_t prod = phi(n / d);
                    prod *= binomial(4 * d, d);
                    prod *= binomial(3 * d, d);
                    prod *= binomial(2 * d, d);
                    sum += prod;
                }
            }
        }
        seq_t denom = (seq_t)(4 * n);
        return sum / denom;
    }
}
