/*
 * A164783  a(n) = 7^n - 6.
 * https://oeis.org/A164783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 6 (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use seq_mul from the utilities to compute powers iteratively.
   This avoids any undefined-linker references to seq_pow.  */
#include <stdint.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute 7^n by repeated multiplication.  */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        result = seq_mul(result, (seq_t)7);
    }
    /* a(n) = 7^n - 6   (n >= 1, so the subtraction is safe). */
    return result - (seq_t)6;
}
