/*
 * A190902  a(n) = Product_{d|n} d*mu(n/d).
 * https://oeis.org/A190902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{d|n} d * mu(n/d) (OEIS A190902)
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For square‑free n = p1 * ... * pk, a(n) = (+1) * n^{2^{k-1}}   (k>1)    */
/* For k = 1 (prime),   a(n) = -n.                                   */
/* For nonsquare‑free n, a(n) = 0.                                   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Special case: a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Check square‑freeness and count distinct prime factors */
    uint64_t temp = n;
    unsigned k = 0;          /* number of distinct prime factors */
    bool squarefree = true;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            if (cnt > 1) {       /* a squared prime factor found */
                squarefree = false;
                break;
            }
            ++k;                 /* distinct prime discovered */
        }
    }
    if (squarefree && temp > 1)
        ++k;                    /* remaining prime factor > sqrt(n) */

    /* Non‑squarefree → 0 */
    if (!squarefree)
        return (seq_t)0;

    /* For any square‑free n > 1, n = product of k primes.   */
    /* The exponent becomes 2^{k-1}, sign negative only for k==1.   */
    seq_t base = (seq_t)n;

    if (k == 1) {                  /* prime case, negative sign */
        seq_t val = big_pow(base, (seq_t)1);  /* n^1  */
        return (seq_t)(-val);
    }

    /* k >= 2 : exponent e = 2^{k-1} */
    seq_t exponent = (seq_t)(1ULL << (k - 1));
    return big_pow(base, exponent);
}
