/*
 * A010209  Continued fraction for sqrt(160).
 * https://oeis.org/A010209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 12. For n > 0, let e = v₂(n) (exponent of 2 in n). Then
 *            a(n) = 24 if e ≥ 3, a(n) = 5 if e == 2, otherwise a(n) = 1
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)12;

    unsigned int e = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        e++;
        tmp >>= 1;
    }

    if (e >= 3)
        return (seq_t)24;
    if (e == 2)
        return (seq_t)5;
    return (seq_t)1;
}
