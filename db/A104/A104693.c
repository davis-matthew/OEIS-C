/*
 * A104693  Numbers n such that in n^3, number of odd digits is larger than
 *          number of even digits.
 * https://oeis.org/A104693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   A104693 – numbers n such that in n³ the decimal representation
 *            contains more odd digits than even digits
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 837, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_to_dec_string.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1 – behaviour below the offset is irrelevant */

    idx_t found = 0;      /* how many qualifying numbers we have seen */
    seq_t cur = 1;        /* candidate integer to test */

    for (;; ++cur)
    {
        seq_t cube = cur * cur * cur;

        /* convert cube to a decimal string */
        char buf[256];
        seq_to_dec_string(cube, buf, sizeof buf);

        /* count odd vs. even digits */
        int diff = 0;      /* odd count – even count */
        for (char *p = buf; *p; ++p)
        {
            int d = *p - '0';
            diff += (d & 1) ? 1 : -1;
        }

        if (diff > 0)      /* more odd digits than even digits */
        {
            ++found;
            if (found == n)
                return cur;
        }
    }
}
