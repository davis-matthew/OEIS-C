/*
 * A106581  Alternating two level four-symbol substitution: characteristic
 *          polynomial: x^2*(x^2-3).
 * https://oeis.org/A106581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106581: alternating blocks of 2 and the repeated pattern 4,3,1.
   The sequence is produced by iteratively subtracting block sizes;
   block 0 contains a single 2, block 1 repeats 4,3,1 once,
   block 2 repeats 2 three times, block 3 repeats 4,3,1 three times, etc.
   The block index alternates A/B, with even indices giving 2,
   odd indices giving 4,3,1 repeated.  */
seq_t a(idx_t n)
{
    idx_t pos   = n;   /* remaining offset within current block       */
    idx_t blk   = 0;   /* block index (0 for the initial single 2)   */
    idx_t size  = 1;   /* current block size (size of block blk)     */

    /* Find the block containing position n.  */
    while (pos >= size)
    {
        pos -= size;   /* skip this whole block                    */
        ++blk;         /* move to next block                       */

        if (blk & 1)   /* next block is a B‑block (size*3)          */
        {
            /* Compute 3*size but avoid overflow.  Use 128‑bit arithmetic,
               and if it would exceed n we can simply set size > pos
               (pos+1) to terminate the loop.                        */
            __int128 next = (__int128)size * 3;
            if (next > (unsigned __int128)n)
                size = pos + 1;  /* ensure next loop condition fails      */
            else
                size = (idx_t)next;
        }
        /* If blk is even the next block is still of the same size.   */
    }

    /* pos now is the index inside block blk.                         */
    if ((blk & 1) == 0)          /* A‑block → always 2                   */
        return (seq_t)2;

    /* B‑block → 4,3,1 repeated.                                      */
    switch (pos % 3)
    {
        case 0: return (seq_t)4;
        case 1: return (seq_t)3;
        default: return (seq_t)1;
    }
}
