/*
 * A178508  a(n) = 1, 7, A011557*(period 6: repeat 10, 13, 31, 49, 70, 97).
 * https://oeis.org/A178508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 7. For n >= 3: let k = floor((n-3)/6) //
 *            exponent of 10 let r = (n-3) mod 6 // position in the pattern
 *            a(n) = 10^k * pattern[r] where pattern = {10, 13, 31, 49, 70,
 *            97}
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms and a bogus index. */
    if (n == 0)        return (seq_t)0;
    if (n == 1)        return (seq_t)1;
    if (n == 2)        return (seq_t)7;

    /* n >= 3 */
    const seq_t pattern[6] = {10, 13, 31, 49, 70, 97};

    idx_t m = n - 3;                /* m >= 0 */
    idx_t k = m / 6;                /* exponent of 10   */
    idx_t r = m % 6;                /* index in pattern */

    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow10 *= (seq_t)10;         /* compute 10^k */

    return pattern[r] * pow10;
}
