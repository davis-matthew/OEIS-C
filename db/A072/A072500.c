/*
 * A072500  Product of divisors of n which are >= n^(1/2).
 * https://oeis.org/A072500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏{ d | n , d ≥ √n } d (product of the divisors of n that
 *            are at least √n)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 119, u64 n <= 359,
 *            u128 n <= 1000, big any n
 * Helpers:   prime_factorization
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/prime_factorization.h"

/* floor integer square root of a 64‑bit unsigned integer */
static uint64_t int_sqrt_uint64(uint64_t v)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= v) ++r;
    return r;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside offset */

    /* factorise n */
    uint64_t pn[64], ex[64];
    size_t pc;
    prime_factorization((uint64_t)n, pn, ex, &pc);

    /* enumerate all divisors of n */
    seq_t divs[256];
    size_t divc = 1;
    divs[0] = (seq_t)1;

    for (size_t i = 0; i < pc; ++i) {
        uint64_t p = pn[i];
        uint64_t e = ex[i];
        size_t cur = divc;
        seq_t pow = (seq_t)1;
        for (uint64_t k = 1; k <= e; ++k) {
            pow *= (seq_t)p;          /* p^k */
            for (size_t j = 0; j < cur; ++j) {
                divs[divc++] = divs[j] * pow;
            }
        }
    }

    seq_t prod = (seq_t)1;
    seq_t N = (seq_t)n;

    for (size_t i = 0; i < divc; ++i) {
        /* include divisor d if d*d >= n */
        if (divs[i] * divs[i] >= N) prod *= divs[i];
    }

    return prod;
}
