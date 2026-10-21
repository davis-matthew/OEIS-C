/*
 * A227800  Number of different values the product p*q can have where p >= 1,
 *          q >= 1 with p+q < n.
 * https://oeis.org/A227800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 806, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: counts distinct products p*q with p>=1, q>=1 and p+q < n
   by brute‑force enumeration.  No simple closed formula is known. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>   /* uint8_t */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    idx_t M = n - 1;                           /* maximum allowed sum p+q */

    /* maximum product occurs when p and q are as close as possible */
    idx_t max_prod = (M / 2) * ((M + 1) / 2);
    size_t sz = (size_t)max_prod + 1;

    /* Use a byte array as a bitset; cast is needed for C++ */
    uint8_t *seen = (uint8_t *)calloc(sz, 1);
    if (!seen)
        return 0;   /* allocation failure → return 0 */

    seq_t count = 0;
    for (idx_t p = 1; p < M; ++p) {
        idx_t max_q = M - p;
        for (idx_t q = 1; q <= max_q; ++q) {
            idx_t prod = p * q;
            if (!seen[(size_t)prod]) {
                seen[(size_t)prod] = 1;
                ++count;
            }
        }
    }

    free(seen);
    return count;
}
