/*
 * A040699  Continued fraction for sqrt(726).
 * https://oeis.org/A040699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 26; for n > 0, a(n) repeats the period [1, 16, 1, 52]
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)26;
    switch (n % 4)
    {
        case 1: return (seq_t)1;
        case 2: return (seq_t)16;
        case 3: return (seq_t)1;
        default: /* n % 4 == 0 */
            return (seq_t)52;
    }
}
