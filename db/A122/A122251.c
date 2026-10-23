/*
 * A122251  Numerators of Hankel transform of 1/(3n+1) (conjecture).
 * https://oeis.org/A122251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^sum_{k=1..n} sum_{j=0..⌊log_3(n)⌋} floor(k/3^j)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    = 9^A122250(n) */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent E = A122250(n) */
    unsigned long long e = 0;          /* exponent fits in 64‑bit for n up to 10000 */
    for (idx_t d = 1; d <= n; d *= 3) {
        unsigned long long q   = n / d;                /* floor(n/d)   */
        unsigned long long part = d * q * (q - 1) / 2; /* first sum block */
        part += q * (n - d * q + 1);                  /* last partial block */
        e += part;
    }

    /* Return 9^e as seq_t (big integer if needed) */
    return big_pow((seq_t)9, (seq_t)e);
}
