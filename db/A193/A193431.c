/*
 * A193431  Put the natural numbers together without spaces and read them
 *          three at a time advancing one space each time.
 * https://oeis.org/A193431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10 * (10 * A007376(n) + A007376(n+1)) + A007376(n+2)
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Return the (1‑based) decimal digit at position pos in the
 * concatenated natural numbers string 123456789101112...   */
static seq_t
digit_at_position(idx_t pos)
{
    seq_t rem = (seq_t)pos;          /* remaining positions to consume   */
    seq_t d   = 1;                   /* current digit length              */

    for (;; ++d)
    {
        /* first number with d digits */
        seq_t start = (d == 1) ? (seq_t)1 : pow10((idx_t)(d - 1));

        /* number of d‑digit numbers */
        seq_t count = (seq_t)9 * start;

        /* total digits contributed by this block */
        seq_t block_digits = count * d;

        if (rem > block_digits)
        {
            rem -= block_digits;      /* skip this whole block */
            continue;
        }

        /* position is inside the current block   */
        seq_t idx_in_block = rem - (seq_t)1;           /* 0‑based              */
        seq_t num_idx      = idx_in_block / d;        /* number offset        */
        seq_t number       = start + num_idx;         /* the actual integer   */
        seq_t digit_idx    = idx_in_block % d;        /* 0‑based digit pos    */

        /* extract the digit at digit_idx (from left) */
        seq_t pow_exp  = (seq_t)(d - 1 - digit_idx);  /* exponent of 10       */
        seq_t pow_val  = pow10((idx_t)pow_exp);
        seq_t digit    = (number / pow_val) % (seq_t)10;

        return digit;
    }
}

seq_t
a(idx_t n)
{
    seq_t d1 = digit_at_position(n);
    seq_t d2 = digit_at_position(n + 1);
    seq_t d3 = digit_at_position(n + 2);

    return d1 * (seq_t)100 + d2 * (seq_t)10 + d3;
}
