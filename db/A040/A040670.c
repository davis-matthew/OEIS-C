/*
 * A040670  Continued fraction for sqrt(697).
 * https://oeis.org/A040670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=26; for n>0, a(n)= [2,2,52] repeating; i.e.,
 *            a(n)=pattern[(n-1)%3]
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) {
        return (seq_t)26;
    }
    /* Repeating partial quotients of continued fraction for sqrt(697). */
    static const seq_t pattern[3] = { (seq_t)2, (seq_t)2, (seq_t)52 };
    return pattern[(n - 1U) % 3U];
}
