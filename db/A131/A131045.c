/*
 * A131045  Binomial transform of Euler's totient function phi(n+1).
 * https://oeis.org/A131045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(n,j) * phi(j+1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 998 (2026-11-01)
 * Helpers:   binomial, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/totient.h"

/* Translated directly from the OEIS definition */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t bin = binomial(n, j);      /* binomial coefficient */
        seq_t ph  = totient(j + 1);       /* Euler's totient of j+1 */
        sum += bin * ph;              /* add the weighted term */
    }
    return sum;
}
