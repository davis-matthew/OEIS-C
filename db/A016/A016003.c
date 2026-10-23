/*
 * A016003  a(n) = (tau(n^8)+7)/8.
 * https://oeis.org/A016003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^8)+7)/8
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include <stdlib.h>

/* Compute a(n) = (tau(n^8)+7)/8.
   Uses factor_u64 to obtain the prime factorisation of n.
   For each prime p^e dividing n, tau(n^8) contains the factor (8*e+1).
   The product of these factors plus 7, divided by 8, yields a(n). */
seq_t a(idx_t n)
{
    /* OEIS sequences start at offset 1; guard against n==0. */
    if (n < 1)
        return 0;

    /* Cast the index to uint64_t for the factorise routine. */
    uint64_t nn = (uint64_t)n;

    /* Factorisation of nn into primes (pr) and exponents (ex).
       At most 64 distinct prime factors for a 64‑bit integer. */
    size_t cap = 64;
    uint64_t *pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t *ex = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!pr || !ex) {
        free(pr);
        free(ex);
        return 0;
    }
    size_t cnt = 0;
    factor_u64(nn, pr, ex, &cnt);

    /* Compute the product of (8*e+1) over all prime powers. */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(8 * ex[i] + 1);
        prod *= term;
    }

    free(pr);
    free(ex);

    /* Return (prod + 7) / 8. */
    return (prod + 7) / 8;
}
