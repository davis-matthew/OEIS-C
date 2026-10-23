/*
 * A039011  Numbers whose base-5 representation has the same number of 1's and
 *          3's.
 * https://oeis.org/A039011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 representation has the same number of 1's and 3's.
   This implementation simply scans integers in increasing order and
   counts how many satisfy the condition until the n‑th one is found. */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;  /* undefined for n < 1, return 0 as safe value */

    /* Count how many qualifying numbers we have seen */
    size_t count = 0;
    /* Iterate over non‑negative integers */
    for (idx_t m = 0; ; ++m) {
        int diff = 0;          /* +1 for each digit 1, -1 for digit 3 */
        idx_t t = m;
        while (t) {
            int d = (int)(t % (idx_t)5);
            if (d == 1)
                ++diff;
            else if (d == 3)
                --diff;
            t /= (idx_t)5;
        }
        if (diff == 0) {       /* m satisfies the condition */
            ++count;
            if (count == (size_t)n)
                return (seq_t)m;
        }
    }
}
