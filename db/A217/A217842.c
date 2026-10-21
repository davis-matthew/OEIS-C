/*
 * A217842  Product of the numbers p such that phi(p) = n, where phi is
 *          Euler's totient function.
 * https://oeis.org/A217842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏{p | φ(p) = n} p 1. Compute φ(p) for all 2 ≤ p ≤ (20 · n
 *            + 100). 2. Multiply all p whose φ(p) equals n. 3. φ(1) = 1, so
 *            for n = 1 the product includes p = 2 only. This reproduces the
 *            OEIS–sequence A217842
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0 is not defined in the sequence – return 0 in that case. */
    if (n < 1) return (seq_t)0;

    /* Upper bound for searching all p such that φ(p) = n.
       Empirically p is always < ~6 n for n ≤ 10⁴; 20 n + 100 is safe. */
    size_t limit = (size_t)n * 20 + 100;
    if (limit < 2) limit = 2;          /* guard for very small n */

    /* Allocate and fill φ array. */
    uint64_t *phi = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!phi) exit(1);                 /* fatal if allocation fails */

    for (size_t i = 0; i <= limit; ++i)
        phi[i] = (uint64_t)i;

    /* Classic sieve for Euler's totient. */
    for (size_t p = 2; p <= limit; ++p)
        if (phi[p] == p)                /* p is prime */
            for (size_t j = p; j <= limit; j += p)
                phi[j] -= phi[j] / p;

    /* Compute the product of all p with φ(p) == n. */
    seq_t prod = 1;
    for (size_t p = 2; p <= limit; ++p)
    {
        idx_t ph = (idx_t)phi[p];
        if (ph == n)
            prod *= (seq_t)p;
    }

    /* φ(1) = 1, so for n = 1 the product automatically contains p = 2 */
    free(phi);
    return prod;
}
