/*
 * A040497  Continued fraction for sqrt(520).
 * https://oeis.org/A040497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=22, a(1)=1; for n≥2, a(n)=4 if n is even, 11 if n is a
 *            multiple of 3, 44 if n is a multiple of both 2 and 3, and 1
 *            otherwise
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)22;
    if (n == 1)
        return (seq_t)1;
    seq_t res = (seq_t)1;
    if (n % (idx_t)2 == 0)
        res *= (seq_t)4;
    if (n % (idx_t)3 == 0)
        res *= (seq_t)11;
    return res;
}
