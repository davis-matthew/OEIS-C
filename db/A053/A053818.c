/*
 * A053818  a(n) = Sum_{k=1..n, gcd(n,k) = 1} k^2.
 * https://oeis.org/A053818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n^2 + (-1)^Ω(n)*rad(n)) * φ(n)) / 6, where Ω(n) is
 *            the number of distinct prime factors, rad(n) their product. a(1)
 *            = 1
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 1580, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 0) return 0;  /* safety for out‑of‑range calls */

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* compute φ(n), rad(n) and r = number of distinct primes */
    seq_t nn = (seq_t)n;
    seq_t phi = nn;
    seq_t rad = 1;
    size_t r = 0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        rad *= p;
        phi = phi / p * (p - 1);
        ++r;
    }

    /* coef = 2*n^2 + (-1)^r * rad  */
    seq_t coef = 2 * nn * nn;
    if (r & 1)            /* odd r => negative sign */
        coef -= rad;
    else
        coef += rad;

    /* a(n) = (coef * φ(n)) / 6  */
    seq_t result = coef * phi;
    result /= 6;

    return result;
}
