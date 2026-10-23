/*
 * A176853  Given a(n) as d digits in length, partition the binary
 *          representation of a(n) into d-digit numbers from left to right and
 *          add them as if they were decimal numbers.
 * https://oeis.org/A176853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of the decimal interpretations of groups of d =
 *            decimal digit count of n, partitioning the binary representation
 *            of n from left to right
 * Range:     u8 n <= 291, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) {
        return (seq_t)0U;
    }

    /* decimal digit count of n */
    int d = 0;
    idx_t tmp = n;
    do {
        d++;
        tmp /= 10U;
    } while (tmp != 0U);

    /* bit length of n */
    int bitlen = 0;
    tmp = n;
    while (tmp != 0U) {
        bitlen++;
        tmp >>= 1U;
    }

    /* number of groups (ceil(bitlen / d)) */
    int groups = (bitlen + d - 1) / d;

    /* shift pointing to most significant bit */
    int shift = bitlen - 1;

    seq_t sum = 0;
    int remaining_groups = groups;
    while (remaining_groups > 0) {
        int group_size;
        if (remaining_groups == 1) {
            /* last group may be shorter */
            int r = bitlen % d;
            group_size = (r == 0) ? d : r;
        } else {
            group_size = d;
        }

        seq_t val = 0;
        for (int i = 0; i < group_size; ++i) {
            /* extract bit, interpreted as decimal digit 0 or 1 */
            seq_t bit = (seq_t)((n >> shift) & 1U);
            val = val * 10U + bit;
            shift--;            /* move to next more‑significant bit */
        }
        sum += val;

        --remaining_groups;
    }

    return sum;
}
