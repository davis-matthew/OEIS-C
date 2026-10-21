/*
 * A052413  Numbers without 5 as a digit.
 * https://oeis.org/A052413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = replace digits d > 4 by d + 1 in the base-9
 *            representation of n‑1
 * Range:     u8 n <= 207, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition; any n ≤ 1 simply returns 0 to avoid
       underflow when computing n-1. */
    if (n <= 1)
        return 0;

    idx_t x = n - 1;        /* work with n‑1 in base‑9 */
    seq_t result = 0;       /* decimal value to return   */
    seq_t place  = 1;       /* current decimal place     */

    /* Extract base‑9 digits, map them, and accumulate the decimal value. */
    while (x > 0) {
        uint64_t d = x % 9;                       /* digit in base‑9  (0…8) */
        seq_t mapped = (seq_t)(d <= 4 ? d : d + 1);/* replace d>4 by d+1   */
        result += mapped * place;                 /* add this digit        */
        place *= 10;                               /* next decimal place    */
        x /= 9;                                    /* shift to next digit   */
    }

    return result;
}
