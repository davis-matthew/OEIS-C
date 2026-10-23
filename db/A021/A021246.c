/*
 * A021246  Decimal expansion of 1/242.
 * https://oeis.org/A021246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of the decimal expansion of 1/242 at position n
 *            (offset 0). The expansion is 0.004132231404958677685950413223...
 *            Starting at n = 2 the digits repeat with period 22. a(0) = 0,
 *            a(1) = 0. For n ≥ 2, a(n) = cycle[(n-2) % 22] where cycle =
 *            {4,1,3,2,2,3,1,4,0,4,9,5,8,6,7,7,6,8,5,9,5,0}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digits that repeat after the initial two zeros */
    static const seq_t cycle[22] = {
        4, 1, 3, 2, 2, 3, 1, 4, 0, 4,
        9, 5, 8, 6, 7, 7, 6, 8, 5, 9,
        5, 0
    };

    if (n < 2) {
        return (seq_t)0;
    }

    /* (n-2) is safe because n >= 2 */
    idx_t idx = (n - 2) % 22;
    return cycle[idx];
}
