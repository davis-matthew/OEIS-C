/*
 * A156827  A001792*A008683.
 * https://oeis.org/A156827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = μ(n) * 2^n * (n + 1) / 4 μ(n) is the Möbius function (±1
 *            for square‑free n, 0 otherwise)
 * Range:     i8 n <= 4, i16 n <= 12, i32 n <= 28, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   _smallest_prime_factor, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* μ(1) = 1 */

    int mu = 1;                           /* Möbius sign */
    idx_t m = n;
    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        int cnt = 0;
        while (m % p == 0) {
            m /= p;
            if (++cnt > 1) return (seq_t)0;   /* square factor → μ = 0 */
        }
        mu = -mu;                         /* flip sign per distinct prime */
    }

    seq_t val = seq_pow((seq_t)2, n);      /* 2^n */
    val = val * (seq_t)(n + 1);           /* * (n + 1) */
    val = val / (seq_t)4;                 /* / 4 */
    return mu < 0 ? -val : val;           /* apply μ(n) */
}
