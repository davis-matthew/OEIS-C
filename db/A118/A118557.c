/*
 * A118557  Numbers beginning with a vowel in French.
 * https://oeis.org/A118557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A118557 – Numbers whose French name starts with a vowel
   (either "un" or "onze").
   a(1) = 1
   a(2) = 11
   Then, the sequence consists of the blocks:
   11000–11999,
   1,000,000–1,999,999,
   11,000,000–11,999,999,
   1,000,000,000–1,999,999,999,
   11,000,000,000–11,999,999,999,
   … (for larger scales)                                               */

seq_t a(idx_t n)
{
    /* handle the two special early terms */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)11;

    /* block of 11000…11999 (size 1000) */
    if (n <= (idx_t)1002) {
        return (seq_t)11000 + (seq_t)(n - (idx_t)3);
    }

    /* n >= 1003: we are in the infinite sequence of blocks
       starting at 1,000,000 and each block contributes 10^k
       numbers (k = 6, 9, 12, …).  The second block in every
       group starts at 11*10^k.                                 */
    idx_t offset = n - (idx_t)1003;   /* position inside the blocks */

    seq_t block_size = (seq_t)1000000;   /* initial block size = 10^6 */

    for (;;) {
        /* first block of this group (multiplier = 1) */
        if (offset < (idx_t)block_size) {
            return (seq_t)1000000 + (seq_t)offset;   /* 1*10^6 + offset */
        }
        offset -= (idx_t)block_size;

        /* second block of this group (multiplier = 11) */
        if (offset < (idx_t)block_size) {
            return 11 * block_size + (seq_t)offset;
        }
        offset -= (idx_t)block_size;

        /* move to the next scale (increase block size by factor 1000) */
        block_size *= (seq_t)1000;
    }
}
