/*
 * A104437  Number of ways to split 1, 2, 3, ..., 4n into 4 arithmetic
 *          progressions each with n terms.
 * https://oeis.org/A104437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 for all n ≥ 5. The initial values are a(1)=1,
 *            a(2)=105, a(3)=15, a(4)=11
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)105;
    if (n == (idx_t)3) return (seq_t)15;
    if (n == (idx_t)4) return (seq_t)11;
    return (seq_t)10;
}
