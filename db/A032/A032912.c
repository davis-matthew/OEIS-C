/*
 * A032912  Numbers whose set of base-5 digits is {1,3}.
 * https://oeis.org/A032912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number whose base‑5 digits are 1 or 3. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the block length L such that
       cumulative_count(L-1) < n <= cumulative_count(L)
       with cumulative_count(k) = 2^(k+1) - 2.  */
    idx_t pow2 = 2;   /* 2^L */
    idx_t len  = 1;   /* current length L */

    while (1) {
        /* cumulative count for length len (i.e., L) */
        idx_t cum = (pow2 << 1) - 2;   /* 2^(L+1) - 2 */
        if (n <= cum)
            break;
        pow2 <<= 1;   /* grow to 2^(L+1) */
        len++;
    }

    /* Offset of n within the block of length len (0‑based). */
    idx_t offset = n - pow2 + 1;   /* 0 <= offset < 2^len */

    seq_t result = 0;
    seq_t base   = 1;              /* 5^i */

    for (idx_t i = 0; i < len; ++i) {
        /* If the i‑th bit of offset is 1 -> digit 3, else 1. */
        if (offset & ((idx_t)1 << i))
            result += (seq_t)3 * base;
        else
            result += base;
        base *= (seq_t)5;
    }

    return result;
}
