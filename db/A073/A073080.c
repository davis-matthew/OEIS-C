/*
 * A073080  3 appears three times, 2*3=6 appears six times, 2*6=12 appears
 *          twelve times etc.
 * https://oeis.org/A073080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) grows in blocks. The first block consists of the value 3
 *            repeated 3 times, the next block contains 6 repeated 6 times,
 *            the next contains 12 repeated 12 times, etc. The k‑th block has
 *            value 3·2^(k‑1) and size 3·2^(k‑1). Thus the cumulative size
 *            after k blocks is 3·(2^k – 1) and a(n) is the va...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not used, sequence is defined for n≥1 */

    idx_t blockSize = 3;          /* value and size of the current block   */
    idx_t remaining = n;

    /* Subtract block sizes until the remaining index falls inside
       the current block.  Each iteration doubles the size/value.     */
    while (remaining > blockSize) {
        remaining -= blockSize;
        blockSize <<= 1;          /* blockSize now equals the next block’s size/value   */
    }

    return (seq_t)blockSize;      /* value of the block that contains index n           */
}
