/*
 * A080593  Consider the standard game of Nim with 3 heaps and make a list of
 *          the losing positions (x,y,z) with x <= y <= z in reverse
 *          lexicographic order; sequence gives x values.
 * https://oeis.org/A080593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the x–coordinate of the n‑th losing Nim position (x,y,z)
 *            with x ≤ y ≤ z sorted by ascending z then ascending y. The value
 *            is obtained by enumerating the triples in that order
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t z = 0; ; ++z)
    {
        for (idx_t y = 0; y <= z; ++y)
        {
            idx_t x = y ^ z;           /* x = y XOR z ensures x⊕y⊕z = 0 */
            if (x <= y)                /* ensures x ≤ y ≤ z */
            {
                if (count == n)
                    return (seq_t)x;   /* start counting from 0 */
                ++count;
            }
        }
    }
}
