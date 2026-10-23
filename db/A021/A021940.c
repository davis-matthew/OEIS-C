/*
 * A021940  Decimal expansion of 1/936.
 * https://oeis.org/A021940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(n)= {0,6,8,3,7,6}[(n-3) mod 6] for n≥3
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial digits before the repeating cycle */
    static const seq_t init[3] = {0, 0, 1};
    if (n < 3)
        return init[n];

    /* Repeating cycle of the decimal expansion of 1/936 */
    static const seq_t cycle[6] = {0, 6, 8, 3, 7, 6};
    /* Compute index in the cycle (n-3) % 6 */
    return cycle[(n - 3) % 6];
}
