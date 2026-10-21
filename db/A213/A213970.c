/*
 * A213970  List of primitive words over the alphabet {1,3}.
 * https://oeis.org/A213970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mask_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A213970
 *  List of primitive words over the alphabet {1,3},
 *  listed in increasing length and lexicographic order.
 *  The function enumerates words of length L by a binary mask,
 *  checks for primitiveness, and returns the nth term. */
#include <stdbool.h>
#include "../utils/mask_to_seq.h"

static bool is_primitive(idx_t mask, idx_t L)
{
    /* test all proper divisors d of L */
    for (idx_t d = 1; d < L; ++d)
    {
        if (L % d != 0)
            continue;

        bool ok = true;
        /* check that the block of length d repeats L/d times */
        for (idx_t p = 0; p < d && ok; ++p)
        {
            idx_t bitp = (mask >> (L - 1 - p)) & 1;
            for (idx_t k = 1; k < L / d; ++k)
            {
                idx_t pos   = p + k * d;
                idx_t bitpos = (mask >> (L - 1 - pos)) & 1;
                if (bitpos != bitp)
                {
                    ok = false;
                    break;
                }
            }
        }
        if (ok)
            return false;  /* found a non‑trivial period → imprimitive */
    }
    return true;   /* no smaller period → primitive */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined; offset is 1 */

    idx_t count = 0;
    for (idx_t L = 1; ; ++L)
    {
        /* For n up to 10,000 L will never exceed 13, so this shift is safe. */
        idx_t max_mask = ((idx_t)1 << L) - 1;
        for (idx_t mask = 0; mask <= max_mask; ++mask)
        {
            if (!is_primitive(mask, L))
                continue;
            ++count;
            if (count == n)
                return mask_to_seq(mask, L);
        }
    }
}
