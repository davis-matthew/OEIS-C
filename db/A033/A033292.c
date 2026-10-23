/*
 * A033292  A Connell-like sequence: take 1 number = 1 (mod Q), 2 numbers = 2
 *          (mod Q), 3 numbers = 3 (mod Q), etc., where Q = 3.
 * https://oeis.org/A033292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = the n-th positive integer x such that x ≡ (group number)
 *            (mod 3) where the group number is the current count of numbers
 *            taken from the next residue class. The rule implements the
 *            description from OEIS A033292
 * Range:     u8 n <= 94, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is 1‑based; a(0) is not defined */
        return (seq_t)0;

    seq_t val = (seq_t)1;           /* current candidate x */
    idx_t group = (idx_t)1;           /* current residue class number */
    idx_t rem = (idx_t)1;             /* numbers still needed from this class */
    idx_t produced = (idx_t)0;        /* how many terms have been output */

    while (1)
    {
        if ((val % (seq_t)3) == (seq_t)(group % (idx_t)3))
        {
            produced++;
            if (produced == n)
                return val;
            rem--;                    /* one number taken from this group */
        }
        val++;                         /* check next integer */

        if (rem == (idx_t)0)           /* finished this group */
        {
            group++;                   /* move to the next residue */
            rem = group;               /* size of the new group */
        }
    }
}
