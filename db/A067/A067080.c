/*
 * A067080  If n = ab...def in decimal notation then the left digitorial
 *          function Ld(n) = ab...def*ab...de*ab...d*...*ab*a.
 * https://oeis.org/A067080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   a(n) = ∏_{k=1}^{⌊log10(n)⌋+1} ⌊n / 10^{k-1}⌋ Recurrence: a(n) =
 *            n * a(floor(n/10)), with a(n)=n for 1≤n≤9
 * Range:     u8 n <= 51, u16 n <= 409, u32 n <= 8142, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t x = n;
    while (x > 0) {
        result *= (seq_t)x;
        x /= 10;
    }
    return result;
}
