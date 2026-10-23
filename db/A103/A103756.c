/*
 * A103756  Number of contiguous digits i in the counting numbers, for i=2.
 * https://oeis.org/A103756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the length of the n-th block of consecutive '2's
   when the natural numbers 1,2,3,… are written concatenated in base 10. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used but harmless */

    idx_t target = n;      /* 1‑based target */
    idx_t found  = 0;      /* number of runs found so far */
    idx_t cur_len = 0;     /* length of the current run of '2's */

    /* process natural numbers one by one */
    for (idx_t num = 1; ; ++num)
    {
        /* extract digits of num into a small stack (most significant first) */
        uint64_t x = (uint64_t)num;
        int d[21];       /* max 20 digits for 64‑bit numbers + safety */
        int dc = 0;
        if (x == 0)
        {
            d[dc++] = 0;
        }
        else
        {
            while (x > 0)
            {
                d[dc++] = (int)(x % 10);
                x /= 10;
            }
        }

        /* walk digits from most significant to least */
        for (int i = dc - 1; i >= 0; --i)
        {
            int dig = d[i];
            if (dig == 2)
            {
                ++cur_len;
            }
            else
            {
                if (cur_len > 0)
                {
                    ++found;
                    if (found == target)
                        return (seq_t)cur_len;
                    cur_len = 0;
                }
            }
        }
        /* continue to next number */
    }
}
