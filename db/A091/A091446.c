/*
 * A091446  Concatenate binary vectors ordered first by length, then by the
 *          number of 1's and finally by an 'unusual' lexicographical ordering
 *          (1 comes before 0).
 * https://oeis.org/A091446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   binomc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of all binary vectors ordered by
   length, then number of 1's, then unusual lexicographic
   ordering (1 < 0).  a(n) returns the nth bit of that
   concatenated sequence (a(0) is the first bit). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomc.h"

seq_t a(idx_t n)
{
    idx_t remaining = n;           /* offset within the infinite string */

    /* Find the length L of the target vector */
    idx_t len = 1;
    for (;; ++len)
    {
        idx_t vectors = (idx_t)1 << len;            /* 2^len */
        idx_t bits_in_len = vectors * len;
        if (remaining >= bits_in_len)
            remaining -= bits_in_len;
        else
            break;
    }

    /* Now 'len' is fixed.  Find the weight W and the vector
       position inside its weight group. */
    idx_t weight = 0;
    idx_t rem_in_len = remaining;

    for (weight = 0; weight <= len; ++weight)
    {
        idx_t cnt_vec = binomc(len, weight);            /* C(len,weight) */
        idx_t bits_in_group = cnt_vec * len;
        if (rem_in_len >= bits_in_group)
            rem_in_len -= bits_in_group;
        else
            break;
    }

    /* rem_in_len now lies within the group of weight 'weight'.  */
    idx_t vec_idx   = rem_in_len / len;      /* 0‑based vector index in this group */
    idx_t bit_pos   = rem_in_len % len;      /* 0‑based bit position (MSB first) */

    /* There are cnt_vec vectors with this weight, sorted by
       decreasing numeric value (unusual lex).  Convert
       vec_idx from descending order to ascending numeric
       order to unrank it. */
    idx_t cnt_vec = binomc(len, weight);
    idx_t rank_desc = vec_idx;
    idx_t rank_asc  = cnt_vec - 1 - rank_desc;   /* ascending numeric rank */

    /* Unrank the binary vector of length 'len' with 'weight' ones,
       in ascending numeric order. */
    idx_t remaining_len = len;
    idx_t remaining_ones = weight;
    uint64_t bits_rep = 0;  /* will contain the vector as a number */

    for (idx_t pos = 0; pos < len; ++pos)
    {
        if (remaining_ones == 0)
            break;  /* all remaining bits are 0 */

        idx_t count_with_zero = binomc(remaining_len - 1, remaining_ones);
        if (rank_asc < count_with_zero)
        {
            /* put 0 at this position */
            bits_rep = bits_rep << 1;
        }
        else
        {
            /* put 1 at this position */
            bits_rep = (bits_rep << 1) | 1;
            rank_asc -= count_with_zero;
            --remaining_ones;
        }
        --remaining_len;
    }

    /* Shift the remaining (last) zeros into place if any. */
    bits_rep <<= remaining_len;

    /* Extract the requested bit.  bit_pos=0 is the MSB. */
    idx_t shift = len - 1 - bit_pos;
    seq_t result = (bits_rep >> shift) & 1u;
    return result;
}
