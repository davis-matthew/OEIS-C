/*
 * A236916  The first "octad" is 0, 1, 2, 2, 2, 2, 3, 3; thereafter add 4 to
 *          get the next octad.
 * https://oeis.org/A236916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pattern[n mod 8] + 4 * floor(n / 8)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where pattern = {0,1,2,2,2,2,3,3} */

seq_t a(idx_t n)
{
    /* index block number and position within the octad */
    idx_t blk = n / 8;
    idx_t pos = n % 8;

    /* the offset contributed by the completed blocks */
    seq_t offset = ((seq_t)blk) * 4;

    /* base pattern for the first octad */
    static const seq_t pattern[8] = {
        (seq_t)0,  /* a(0)   */
        (seq_t)1,  /* a(1)   */
        (seq_t)2,  /* a(2)   */
        (seq_t)2,  /* a(3)   */
        (seq_t)2,  /* a(4)   */
        (seq_t)2,  /* a(5)   */
        (seq_t)3,  /* a(6)   */
        (seq_t)3   /* a(7)   */
    };

    return pattern[pos] + offset;
}
