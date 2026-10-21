/*
 * A072834  Exponents occurring in expansion of F_8(q^2).
 * https://oeis.org/A072834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + pa(n mod 4), where pa(0)=0, pa(1)=3, pa(2)=4,
 *            pa(3)=3
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pa[4] = {0, 3, 4, 3};
    const idx_t r = n & 3;              /* n % 4 */
    return ((seq_t)n * ((seq_t)4)) + pa[r];
}
