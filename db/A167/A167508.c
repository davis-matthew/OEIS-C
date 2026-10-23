/*
 * A167508  Number of different letters in the French spelling of the number
 *          n.
 * https://oeis.org/A167508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, changed
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   add_sub, add_word_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of distinct letters in the French spelling of n
   (letters a–z only, accents and punctuation ignored).  The function
   follows the standard French numbering rules up to arbitrary size. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/add_sub.h"
#include "../utils/add_word_mask.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t val = (uint64_t)n;
    uint32_t mask = 0;

    if (val == 0) {
        add_word_mask("zero", &mask);
    } else {
        /* Split into groups of 3 digits (units, thousands, millions, …) */
        uint32_t groups[10];
        int gcnt = 0;
        while (val > 0 && gcnt < 10) {
            groups[gcnt++] = (uint32_t)(val % 1000);
            val /= 1000;
        }

        /* Names for powers of 1000 starting with million (index 2) */
        static const char *power_name[6] = {
            "million", "milliard", "billion",
            "billiard", "trillion", "trilliard"
        };

        for (int i = gcnt - 1; i >= 0; --i) {
            uint32_t g = groups[i];
            if (g == 0)
                continue;

            if (i == 0) {                       /* units */
                add_sub(g, &mask);
            } else if (i == 1) {                /* thousands */
                if (g == 1)
                    add_word_mask("mille", &mask);
                else {
                    add_sub(g, &mask);
                    add_word_mask("mille", &mask);
                }
            } else {                            /* millions and higher */
                add_sub(g, &mask);
                if (i - 2 < (int)(sizeof(power_name)/sizeof(power_name[0]))) {
                    add_word_mask(power_name[i - 2], &mask);
                    if (g > 1)
                        mask |= 1u << ('s' - 'a');
                }
            }
        }
    }

    /* Count set bits in mask = number of distinct letters */
    int cnt = 0;
    uint32_t m = mask;
    while (m) {
        m &= m - 1;
        ++cnt;
    }
    return (seq_t)cnt;
}
