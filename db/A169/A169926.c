/*
 * A169926  Values of n >= 0 such that 3*n-45+360/(n/2+8) is an integer.
 * https://oeis.org/A169926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence formula: a(n) are all non‑negative integers n for which  
   3*n - 45 + 360/(n/2 + 8) is an integer.  The sequence is finite and
   consists of the values listed in the OEIS entry; for n > 19 the
   function returns 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence has exactly 19 terms.  We generate them once
       in ascending order. */
    static size_t count = 0;          /* number of terms stored */
    static seq_t seq[19];             /* sorted term values */

    if (count == 0)
    {
        /* Even n: 2*d - 16 for each divisor d of 360 with d >= 8. */
        for (idx_t d = 8; d <= 360; ++d)
        {
            if (360 % d == 0)
            {
                seq[count++] = (seq_t)2 * (seq_t)d - 16;
            }
        }
        /* Odd solution: n = 29 (the only odd n satisfying the
           condition).  Insert it into the sorted array. */
        size_t i = 0;
        while (i < count && seq[i] < 29) ++i;
        for (size_t j = count; j > i; --j)
            seq[j] = seq[j-1];
        seq[i] = 29;
        ++count;
    }

    if (n == 0)        /* offset is 1, but protect against 0 index */
        return 0;
    if (n > count)     /* undefined beyond the list; return 0 */
        return 0;

    return seq[n-1];
}
