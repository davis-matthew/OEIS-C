/*
 * A053292  Number of nonsingular n X n matrices over GF(5).
 * https://oeis.org/A053292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n – 1)*(5^n – 5)*...*(5^n – 5^(n‑1)) (empty product
 *            for n = 0 gives 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 5^n */
    seq_t pow5_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow5_n *= (seq_t)5;

    seq_t result = (seq_t)1;
    seq_t pow5_k = (seq_t)1;          /* 5^0 at start */
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t term = pow5_n - pow5_k; /* 5^n – 5^k */
        result *= term;
        pow5_k *= (seq_t)5;           /* advance to next power of 5 */
    }
    return result;
}
