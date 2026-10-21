/*
 * A052008  a(n) = 'n with digits sorted in ascending order' + 'n with digits
 *          sorted in descending order'.
 * https://oeis.org/A052008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   a(n) = A004185(n) + A004186(n) (digits of n sorted ascending +
 *            descending)
 * Range:     u8 n <= 102, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    int count[10] = {0};

    /* Count digits of n */
    if (t == 0) {
        count[0] = 1;
    } else {
        while (t > 0) {
            int d = t % 10;
            count[d]++;
            t /= 10;
        }
    }

    /* Construct ascending and descending numbers */
    seq_t asc = 0;
    for (int d = 0; d <= 9; ++d) {
        for (int i = 0; i < count[d]; ++i) {
            asc = asc * 10 + d;
        }
    }
    seq_t desc = 0;
    for (int d = 9; d >= 0; --d) {
        for (int i = 0; i < count[d]; ++i) {
            desc = desc * 10 + d;
        }
    }
    return asc + desc;
}
