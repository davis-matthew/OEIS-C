/*
 * A040205  Continued fraction for sqrt(220).
 * https://oeis.org/A040205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=14; for n>=1, a(n)=1 if n is odd, 4 if n≡2 (mod 4), 28 if
 *            n≡0 (mod 4)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 14;
    idx_t e = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        e++;
        tmp >>= 1;
    }
    if (e == 0) return 1;
    if (e == 1) return 4;
    return 28;
}
