/*
 * A039136  Numbers whose base-10 representation has the same number of 3's
 *          and 5's.
 * https://oeis.org/A039136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039136: compute by scanning natural numbers and selecting those
   whose decimal representation contains an equal number of 3's and 5's. */

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence has offset 1, a(1)=0 */

    idx_t  count   = 0;     /* number of found terms */
    seq_t  candidate = 0;   /* current number being tested */

    for (;;)
    {
        seq_t t = candidate;
        unsigned c3 = 0, c5 = 0;

        /* Count digits 3 and 5 in candidate */
        while (t > 0)
        {
            unsigned d = (unsigned)(t % 10);
            if (d == 3) ++c3;
            else if (d == 5) ++c5;
            t /= 10;
        }

        if (c3 == c5)
        {
            ++count;
            if (count == n) return candidate;
        }

        ++candidate;
    }
}
