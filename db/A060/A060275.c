/*
 * A060275  At least two ordered triples of positive numbers have sum n and
 *          equal products.
 * https://oeis.org/A060275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=13, a(2)=14, a(3)=16, a(4)=17, a(n)=n+14 for n≥5 (i.e. all
 *            integers ≥13 except 15 and 18)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 13 + (seq_t)(n - 1);
    if (n == 3) return 16;
    if (n == 4) return 17;
    return (seq_t)n + (seq_t)14;
}
