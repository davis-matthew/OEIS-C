/*
 * A163327  Self-inverse permutation of integers: swap the odd- and
 *          even-positioned digits in the ternary expansion of n, then convert
 *          back to decimal.
 * https://oeis.org/A163327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ∑_{k≥0} f((n/9^k) mod 9) * 9^k, where f is the digit
 *            mapping {0→0, 1→3, 2→6, 3→1, 4→4, 5→7, 6→2, 7→5, 8→8}
 * Range:     u8 n <= 89, u16 n <= 728, u32 n <= 728, u64 n <= 728,
 *            u128 n <= 728, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int map[9] = {0, 3, 6, 1, 4, 7, 2, 5, 8};
    seq_t result = 0;
    seq_t base   = 1;          /* 9^k */

    while (n > 0)
    {
        idx_t digit   = n % 9;          /* current base‑9 digit */
        int   mapped  = map[digit];     /* mapped digit */
        result += (seq_t)mapped * base;/* add new digit's contribution */
        base   *= (seq_t)9;             /* next power of 9 */
        n      /= 9;
    }

    return result;
}
