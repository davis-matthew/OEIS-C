/*
 * A169929  Integer values of 10*n-122+360/n for n >= 7 (see A169926).
 * https://oeis.org/A169929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 10*m - 122 + 360/m, where m is the nth divisor of 360
 *            that is ≥ 8 (sorted ascending)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t divs[] = {8,9,10,12,15,18,20,24,30,36,40,45,60,72,90,120,180,360};
    if (n == 0) return 0;
    if (n > sizeof divs / sizeof divs[0]) return 0;        /* undefined indices return 0 */
    idx_t d = divs[n-1];
    seq_t dn = (seq_t)d;
    seq_t term = dn * 10 - 122 + (seq_t)(360 / d);
    return term;
}
