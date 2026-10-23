/*
 * A040754  Continued fraction for sqrt(782).
 * https://oeis.org/A040754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* formula: a(n) = 27 if n=0; a(n)=1 if n is odd; a(n)=26 if n is even but not divisible by 4; a(n)=54 if n is divisible by 4 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)27;
    if ((n & 1) != 0) return (seq_t)1;
    if ((n & 3) == 2) return (seq_t)26;
    return (seq_t)54;
}
