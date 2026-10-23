/*
 * A108782  Difference between n and the largest number with the same digit
 *          set as n.
 * https://oeis.org/A108782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A004186(n) - n
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* count decimal digits of n */
    unsigned cnt[10] = {0};
    seq_t tmp = (seq_t)n;
    if (tmp == 0) {
        cnt[0] = 1;
    } else {
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 10);
            cnt[d]++;
            tmp /= 10;
        }
    }

    /* build the largest permutation of those digits */
    seq_t maxnum = 0;
    for (int d = 9; d >= 0; d--) {
        for (unsigned i = 0; i < cnt[d]; i++) {
            maxnum = maxnum * 10 + (seq_t)d;
        }
    }

    /* return difference */
    return maxnum - (seq_t)n;
}
