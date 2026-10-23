/*
 * A119464  Consider the standard game of Nim with 3 heaps and make a list of
 *          the losing positions (x,y,z) with x <= y <= z sorted by sum, ties
 *          broken by putting smallest value of x first, then y, then z;
 *          sequence gives x values.
 * https://oeis.org/A119464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the x–coordinate of the nth Nim losing triple (x,y,z)
 *            with x ≤ y ≤ z sorted by sum, then by x, then y, then z
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Do not include anything else.  This function can be called many times
   with arbitrary order of n.  It generates all losing triples until
   the nth is found.  Complexities are modest for n up to several
   thousands.  */
seq_t a(idx_t n)
{
    /*  counter counts how many triples have been seen so far */
    idx_t counter = 0;
    for (idx_t sum = 0; ; ++sum )          /* increase total sum progressively   */
    {
        for (idx_t x = 0; x <= sum; ++x)   /* x ranges 0 .. sum   */
        {
            for (idx_t y = x; y <= sum - x; ++y)  /* y ≥ x, and y ≤ sum - x */
            {
                idx_t z = sum - x - y;                /* z determined automatically   */
                if (y > z) break;                    /* maintain ordering x ≤ y ≤ z   */
                seq_t xs = (seq_t)x;
                seq_t ys = (seq_t)y;
                seq_t zs = (seq_t)z;
                if ( (xs ^ ys ^ zs) == 0 )           /* P-position condition   */
                {
                    ++counter;
                    if (counter == n) return xs;    /* nth term found          */
                }
            }
        }
    }
}
