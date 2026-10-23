/*
 * A151665  G.f.: Product_{k>=0} (1 + 3*x^(4^k)).
 * https://oeis.org/A151665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^{count_ones_4(n)} when the base‑4 digits of n are all 0
 *            or 1; otherwise a(n) = 0. Here count_ones_4(n) is the number of
 *            1‑digits in the base‑4 expansion of n
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    while (n > 0)
    {
        uint64_t digit = n & 3ULL;   /* lowest two bits = base‑4 digit */
        if (digit > 1)
            return 0;                /* digit 2 or 3 → coefficient 0 */
        if (digit == 1)
            res *= 3;                /* each 1 contributes a factor 3 */
        n >>= 2;                    /* move to next 4‑ary digit */
    }
    return res;
}
