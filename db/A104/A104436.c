/*
 * A104436  Number of ways to split 1, 2, 3, ..., 3n into 3 arithmetic
 *          progressions each with n terms.
 * https://oeis.org/A104436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=15, a(3)=5, a(n)=4 for all n≥4
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)5;
    return (seq_t)4;
}
