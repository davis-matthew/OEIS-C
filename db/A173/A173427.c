/*
 * A173427  Decimal value a(n) of the binary number b(n) obtained by starting
 *          from 1, sequentially concatenating all binary numbers up to n and
 *          then sequentially concatenating all binary numbers from n-1 down
 *          to 1.
 * https://oeis.org/A173427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal value of the binary number *
 *            concatenate(1,10,11,…,binary(n),…,11,10,1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

/* Computes a(n) for n >= 1. */
seq_t a(idx_t n)
{
    seq_t res = 0;                     /* current binary value */
    idx_t i, len;

    /* Upward concatenation: 1, 2, …, n */
    for (i = 1; i <= n; ++i) {
        len = floor_log2_uint64(i) + 1; /* length of binary(i) */
        res = (res << len) | (seq_t)i;
    }

    /* Downward concatenation: n-1, n-2, …, 1 */
    for (i = n - 1; i >= 1; --i) {
        len = floor_log2_uint64(i) + 1;
        res = (res << len) | (seq_t)i;
        if (i == 1) break;            /* avoid unsigned underflow */
    }

    return res;
}
