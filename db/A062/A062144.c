/*
 * A062144  Sixth (unsigned) column sequence of coefficient triangle A062137
 *          of generalized Laguerre polynomials n!*L(n,3,x).
 * https://oeis.org/A062144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)! * binomial(n+8,8) / 120
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   legendre
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/legendre.h"

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return (seq_t)1;

    idx_t limit = n + 8;                /* maximum number we need primes for   */
    size_t sz = (size_t)limit + 1;      /* array size for sieve                */

    bool *isPrime = (bool *)calloc(sz, sizeof(bool));
    if (!isPrime) return (seq_t)0;      /* allocation failure (unlikely)        */

    /* Simple sieve of Eratosthenes */
    for (idx_t i = 2; i < (idx_t)sz; ++i) isPrime[i] = true;
    for (idx_t p = 2; (idx_t)p * (idx_t)p < (idx_t)sz; ++p) {
        if (isPrime[p]) {
            for (idx_t j = p * p; j < (idx_t)sz; j += p)
                isPrime[j] = false;
        }
    }

    seq_t result = (seq_t)1;

    /* For each prime p ≤ limit compute its exponent in a(n) */
    for (idx_t p = 2; p < (idx_t)sz; ++p) {
        if (!isPrime[p]) continue;

        /* Legendre's formula for v_p(k!) */
        idx_t vp_n5 = legendre(n + 5, p);
        idx_t vp_n8 = legendre(n + 8, p);
        idx_t vp_n  = legendre(n, p);

        /* Subtract factors coming from 8! and 120 = 5!   */
        idx_t sub = 0;
        if (p == 2) sub = 10;   /* v_2(8!) + v_2(120) = 7 + 3 = 10 */
        else if (p == 3) sub = 3;/* v_3(8!) + v_3(120) = 2 + 1 = 3  */
        else if (p == 5) sub = 2;/* v_5(8!) + v_5(120) = 1 + 1 = 2  */
        else if (p == 7) sub = 1;/* v_7(8!) + v_7(120) = 1 + 0 = 1  */

        idx_t e = vp_n5 + vp_n8 - vp_n - sub;
        if (e > 0) {
            for (idx_t i = 0; i < e; ++i)
                result *= (seq_t)p;
        }
    }

    free(isPrime);
    return result;
}
