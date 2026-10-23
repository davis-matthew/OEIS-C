/*
 * A045244  Numbers whose base-5 representation contains exactly three 1's and
 *          no 3's.
 * https://oeis.org/A045244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A045244
 *  Numbers whose base-5 representation contains exactly three 1's and no 3's
 *  Implementation: brute‑force search.  For any n≥1 it scans integers in
 *  increasing order and tests the base‑5 digits until the nth qualifying
 *  number is found.  This method is O(a(n))  and is easily fast enough for
 *  n up to about 10 000 (the 10 000th term is < 10 million).  It works for
 *  all supported seq_t types, including 8/16/32/64‑bit unsigned integers
 *  and arbitrary‑precision integer classes.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* the sequence is defined for n≥1; behaviour below is undefined */
        return (seq_t)0;

    idx_t count = 0;
    seq_t x = 0;

    for (;; ++x)
    {
        seq_t tmp = x;
        int ones = 0;
        bool ok = true;

        /* examine the base‑5 digits of x */
        while (tmp > (seq_t)0)
        {
            int d = (int)(tmp % (seq_t)5);
            if (d == 3) { ok = false; break; }
            if (d == 1) ++ones;
            tmp /= (seq_t)5;
        }

        if (!ok)
            continue;

        if (ones == 3)
        {
            ++count;
            if (count == n)
                return x;
        }
    }
}
