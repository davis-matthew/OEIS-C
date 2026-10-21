/*
 * A088721  Unique monotone sequence satisfying a(a(a(a(n)))) = 2n.
 * https://oeis.org/A088721
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the unique monotone sequence satisfying a(a(a(a(n)))) =
 *            2n. First differences are 1^3, 2^1, 1^6, 2^2, 1^12, 2^4, …
 *            (powers of 2)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)   /* offset is 4; behaviour for n<4 does not matter */
        return 0;

    /* offset from the first defined index */
    idx_t offset = n - 4;

    /* a(4) = 5 */
    seq_t sum = (seq_t)5;

    /* block parameters for j=0 */
    unsigned long long block_len = 4;   /* 2^(j+2) */
    unsigned long long ones     = 3;    /* 3*2^j  */
    unsigned long long twos     = 1;    /*   2^j  */

    while (offset > 0) {
        if (offset >= block_len) {
            /* whole block fits */
            sum += (seq_t)(ones + 2 * twos); /* ones*1 + twos*2 */
            offset -= block_len;
        } else {
            /* partial block – add ones part */
            if (offset <= ones) {
                sum += (seq_t)offset;            /* all remaining are 1’s */
            } else {
                sum += (seq_t)ones;              /* all 1’s */
                unsigned long long rem = offset - ones;
                sum += (seq_t)(rem * 2);         /* remaining are 2’s */
            }
            offset = 0;
        }
        block_len <<= 1;   /* next block length (double) */
        ones     <<= 1;    /* next ones count */
        twos     <<= 1;    /* next twos count */
    }
    return sum;
}
