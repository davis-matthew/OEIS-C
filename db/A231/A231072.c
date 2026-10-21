/*
 * A231072  Number of words in English spelling of n.
 * https://oeis.org/A231072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* By counting words of English spelling (American style). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for zero */
    if (n == 0)
        return (seq_t)1;

    seq_t words = 0;
    idx_t temp = n;
    int groupIndex = 0;                /* 0 -> units, 1 -> thousand, 2 -> million, etc. */

    while (temp > 0)
    {
        idx_t group = temp % (idx_t)1000;
        temp /= (idx_t)1000;

        if (group > 0)
        {
            /* Count words for the 3‑digit group */
            seq_t w = 0;
            idx_t hundreds = group / (idx_t)100;
            idx_t rem      = group % (idx_t)100;

            if (hundreds > 0)
            {                /* digit word + "hundred" */
                w += 2;
            }

            if (rem > 0)
            {
                if (rem <= 20)
                    w += 1;           /* words for 1–20: one word */
                else
                {
                    w += 1;          /* tens word */
                    if (rem % 10)
                        w += 1;      /* ones word if not zero */
                }
            }

            words += w;

            /* Add the name of the power of thousand if this is not the units group */
            if (groupIndex > 0)
                words += 1;          /* "thousand", "million", ... are single words */
        }

        groupIndex++;
    }

    return words;
}
