/*
 * A138648  7^n mod 5^n.
 * https://oeis.org/A138648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod 5^n (with a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   ipow, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t mod = ipow((seq_t)5, (seq_t)n);   /* compute 5^n */
    return powmod((seq_t)7, n, mod);           /* compute 7^n mod 5^n */
}
