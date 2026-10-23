/*
 * A039613  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 11's.
 * https://oeis.org/A039613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039613: Numbers whose base‑12 representation has the same nonzero
 * number of digits 1 and 11 (B).  Returns the n‑th such number (a(1)=23)
 * by a simple linear search. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 0;
    while (found < n)
    {
        x++;                       /* next candidate */
        seq_t t = x;
        unsigned cnt1 = 0, cntB = 0;
        while (t > 0)
        {
            seq_t d = t % 12;      /* current base‑12 digit */
            if (d == 1) cnt1++;
            else if (d == 11) cntB++;
            t /= 12;
        }
        if (cnt1 > 0 && cnt1 == cntB)
            found++;
    }
    return x;
}
