/*
 * A080594  Consider the standard game of Nim with 3 heaps and make a list of
 *          the losing positions (x,y,z) with x <= y <= z in reverse
 *          lexicographic order; sequence gives y values.
 * https://oeis.org/A080594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the middle heap size y of the n‑th losing Nim triple
 *            (x,y,z) with 0 ≤ x ≤ y ≤ z and x ⊕ y ⊕ z = 0, when the triples
 *            are ordered by increasing z and, for equal z, by increasing y
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of triples seen so far */
    for (idx_t z = 0;; ++z)   /* iterate over possible z values */
    {
        for (idx_t y = 0; y <= z; ++y)   /* possible y values for this z */
        {
            seq_t x = ((seq_t)z) ^ ((seq_t)y);  /* x satisfies x ⊕ y = z */
            if (x <= y)                       /* ensure x ≤ y ≤ z */
            {
                if (count == n)               /* desired index reached */
                    return (seq_t)y;          /* return y as seq_t */
                ++count;                       /* advance to next triple */
            }
        }
    }
}
