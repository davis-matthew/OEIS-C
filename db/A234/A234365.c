/*
 * A234365  Number of directed Hamiltonian cycles in the complete tripartite
 *          graph K_{n,n,n}.
 * https://oeis.org/A234365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!^2 * (n-1)! * A141147(n), with A141147(n) =
 *            Sum_{k=0..floor(n/2)} C(n,2k)*C(2k,k)*C(n-1+k,k)*2^(n-2k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 152 (2026-11-07)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t A141147_a(idx_t n)
{
    seq_t result = 0;
    idx_t half = n >> 1;
    for (idx_t k = 0; k <= half; ++k) {
        seq_t prod = binomial(n, 2 * k);
        prod *= binomial(2 * k, k);
        prod *= binomial(n - 1 + k, k);
        prod *= pow2(n - 2 * k);
        result += prod;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t f = 1;                 /* (n-1)! */
    for (idx_t i = 2; i < n; ++i) f *= (seq_t)i;
    seq_t nf = f * (seq_t)n;     /* n! */
    seq_t r = A141147_a(n);
    r *= nf;
    r *= nf;
    r *= f;
    return r;
}
