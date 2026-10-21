/*
 * A214214  Partial sums of A214212.
 * https://oeis.org/A214214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^{n} A214212(k) * A214212(k) takes only the
 *            values 2 and 4. * The sequence of differences of a(n) (i.e.
 *            A214212) consists of alternating * blocks of 2's and 4's whose
 *            lengths double each time: * 2,2, * 4,2, * 4,4, * 2,2,4,4,4,4, *
 *            2,2,2,2,4,4,4,4,4,4,4,4,… * Thus after an initia...
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 1;          /* a(0) */
    idx_t  remaining = n;   /* how many A214212 terms we still need */

    /* Initial block of two 2's.  Length 2. */
    idx_t block_len = 2;
    if (remaining < block_len) {
        sum += (seq_t)remaining * 2;
        return sum;
    }
    sum += (seq_t)block_len * 2;
    remaining -= block_len;

    /* Process the alternating 4/2 blocks.  The block length starts at 1
       and doubles after each pair of 4 and 2 blocks. */
    idx_t len = 1;
    while (remaining > 0) {
        /* 4 block */
        idx_t take = (remaining < len) ? remaining : len;
        sum += (seq_t)take * 4;
        remaining -= take;
        if (remaining == 0)
            break;

        /* 2 block */
        take = (remaining < len) ? remaining : len;
        sum += (seq_t)take * 2;
        remaining -= take;
        if (remaining == 0)
            break;

        len <<= 1;        /* double the block length for the next pair */
    }

    return sum;
}
