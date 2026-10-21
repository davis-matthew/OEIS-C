/*
 * A217123  Number of possible ordered pairs (x, y) where x is the number of
 *          beads adjacent to at least one black bead and y the number of
 *          beads adjacent to at least one white bead in a binary necklace of
 *          length n.
 * https://oeis.org/A217123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*floor(n/4)^2 + 2*floor(n/4)*(n mod 4) + (n mod 4==3 ? 4
 *            : 2) for n>=4, a(1)=2, a(2)=3, a(3)=4
 * Range:     u8 n <= 31, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    idx_t p = n / 4;
    idx_t q = n % 4;
    idx_t base = 4 * p * p + 2 * p * q;
    idx_t offset = (q == 3) ? 4 : 2;
    return (seq_t)(base + offset);
}
