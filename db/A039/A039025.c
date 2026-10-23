/*
 * A039025  Numbers whose base-6 representation has the same number of 2's and
 *          4's.
 * https://oeis.org/A039025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   The sequence consists of all non‑negative integers whose *
 *            base‑6 representation contains an equal number of digit 2’s and
 *            4’s
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1, but we guard against 0 for safety. */
    if (n == 0) return (seq_t)0;

    idx_t  found = 0;     /* number of qualifying numbers seen so far   */
    seq_t  x     = 0;     /* current number being examined              */

    for (;; ++x)          /* loop until we find the n‑th term            */
    {
        seq_t  xx   = x;
        unsigned cnt2 = 0, cnt4 = 0;

        /* count 2's and 4's in base‑6 representation of xx */
        while (xx > 0)
        {
            seq_t d = xx % (seq_t)6;
            if (d == (seq_t)2) ++cnt2;
            else if (d == (seq_t)4) ++cnt4;
            xx /= (seq_t)6;
        }

        /* 0 has no digits, so cnt2 == cnt4 == 0, which is correct. */
        if (cnt2 == cnt4)
        {
            ++found;
            if (found == n) return x;
        }
    }
}
