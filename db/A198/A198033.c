/*
 * A198033  Greatest residue of x^n (mod 2*n+1), x=0..2*n.
 * https://oeis.org/A198033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{x=0..2*n} x^n mod (2*n+1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)(2 * n + 1);          /* modulus m = 2*n+1 */
    seq_t max = 0;                           /* maximum residue found */
    for (idx_t x = 0; x <= 2 * n; ++x) {
        seq_t r = mod_pow((seq_t)x, n, mod);
        if (r > max) {
            max = r;
        }
    }
    return max;
}
