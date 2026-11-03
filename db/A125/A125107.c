/*
 * A125107  Subtract compositions (A011782) from Catalan numbers (A000108).
 * https://oeis.org/A125107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n) - 2^(n-1) (a(0)=0)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t c = catalan(n);   /* Catalan number C_n */
    seq_t p = pow2(n - 1);  /* 2^(n-1) */
    return c - p;
}
