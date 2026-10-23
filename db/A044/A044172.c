/*
 * A044172  Numbers n such that string 4,6 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044172: numbers n whose base‑7 representation contains the substring
   46, but the representation of n‑1 does not.  Computed by iterating and
   testing each candidate. */
#include <stdbool.h>

/* true if x in base 7 contains the adjacent digits “4” followed by “6” (in
   normal order).  The test is performed by scanning the least‑significant
   digit first. */
static bool has46(seq_t x)
{
    seq_t prev = (seq_t)-1;          /* sentinel for "no previous digit"   */
    while (x > 0)
    {
        seq_t digit = x % 7;
        if (digit == 4 && prev == 6)   /* we saw a 6 then a 4 -> pattern 46 */
            return true;
        prev = digit;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* behaviour below offset is irrelevant */

    idx_t count = 0;
    idx_t m     = 0;                /* current candidate                     */
    bool prev_has = false;          /* has46(m‑1) for m==1 is false           */

    while (count < n)
    {
        ++m;
        bool curr_has = has46((seq_t)m);
        if (curr_has && !prev_has)
            ++count;

        if (count == n)
            return (seq_t)m;

        prev_has = curr_has;
    }
    return 0;                       /* should never reach this line */
}
