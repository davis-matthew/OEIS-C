/*
 * A019590  Fermat's Last Theorem: a(n) = 1 if x^n + y^n = z^n has a
 *          nontrivial solution in integers, otherwise a(n) = 0.
 * https://oeis.org/A019590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = 1 if n==1 or n==2, otherwise 0 * (Fermat's Last Theorem:
 *            only non-trivial integer solutions exist for n=1,2)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 1 || n == 2) ? 1 : 0;
}
