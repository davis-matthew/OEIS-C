/*
 * A040762  Continued fraction for sqrt(791).
 * https://oeis.org/A040762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=28; for n>=1, a(n)=8 if n odd, 56 if n even */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;
    return (n & 1U) ? (seq_t)8 : (seq_t)56;
}
