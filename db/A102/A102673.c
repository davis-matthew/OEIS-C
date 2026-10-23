/*
 * A102673  Number of digits >= 4 in decimal representation of n.
 * https://oeis.org/A102673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of decimal digits of n that are ≥ 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;            /* use the 64‑bit index for digit extraction */
    seq_t cnt = 0;          /* count of digits ≥ 4 */

    /* Handle n = 0 explicitly – its single digit 0 is < 4 */
    if (x == 0)
        return 0;

    /* Count digits while extracting them */
    while (x > 0) {
        uint8_t d = x % 10;
        if (d >= 4)
            cnt++;          /* increment only when digit ≥ 4 */
        x /= 10;
    }

    return cnt;             /* result fits easily into seq_t */
}
