/*
 * A164349  The limit of the string "0, 1" under the operation 'repeat string
 *          twice and remove last symbol'.
 * https://oeis.org/A164349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1. For n>1, let k be the smallest integer such that
 *            2^k+1>n. Then repeatedly subtract 2^{k-1}+1 from the index while
 *            k>0. The remaining index is 0 or 1, which is the sequence value
 * Range:     u8 n <= 9088, u16 n <= 9088, u32 n <= 9088, u64 n <= 9088,
 *            u128 n <= 9088, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    idx_t i = n;
    /* find the smallest k with 2^k > n-1 */
    idx_t base = 1;     /* 2^0 */
    idx_t k = 0;
    while (base <= i - 1) {
        base <<= 1;
        ++k;
    }

    /* walk down from level k to level 0 */
    while (k > 0) {
        idx_t lenPrev = (base >> 1) + 1; /* 2^{k-1} + 1 */
        if (i >= lenPrev)
            i -= lenPrev;
        base >>= 1;
        --k;
    }

    /* i is now 0 or 1 */
    return (seq_t)(i);
}
