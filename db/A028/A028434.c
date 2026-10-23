/*
 * A028434  Golc sequence in base 4. Left to right concatenation of
 *          n,int(log_4(n)),int(log_4(int(log_4(n)))),... in base 4.
 * https://oeis.org/A028434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by concatenating, in base‑4, the number n, *
 *            floor(log₄(n)), floor(log₄(floor(log₄(n)))), …, until a zero
 *            value * would be appended after a block that is strictly larger
 *            than 1. * In practice the sequence is computed by building the
 *            decimal value of * the base‑4 concatenation of...
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_log2_uint64, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow4.h"
#include "../utils/floor_log2_uint64.h"

static inline int digits_in_base4(idx_t v)
{
    if (v == 0U) return 1;
    return ((int)floor_log2_uint64(v) >> 1) + 1;
}

seq_t a(idx_t n)
{
    /* handle the special case n==1 */
    if (n == 0U) return 0;
    if (n == 1U) return 4;

    seq_t result = n;          /* first block is the base‑4 representation of n,
                                 whose decimal value equals n itself           */
    idx_t curr = n;

    while (1)
    {
        /* compute next block: floor(log4(curr)) */
        idx_t next = floor_log2_uint64(curr) >> 1;

        if (next == 0U)
        {
            /* append zero only if the previous block was >1 */
            if (curr > 1U)
                result = result * pow4(1U);   /* +0 is implicit */
            break;
        }

        int d = digits_in_base4(next);
        result = result * pow4((idx_t)d) + next;
        curr = next;
    }

    return result;
}
