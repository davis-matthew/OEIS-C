/*
 * A104777  Integer squares congruent to 1 mod 6.
 * https://oeis.org/A104777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n - 1 - (n & 1))^2 (sq of odd ints not divisible by 3)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n≥1 the formula holds; behaviour for n=0 is undefined (offset). */
    seq_t nn = (seq_t)n;
    seq_t t  = 3 * nn - 1 - (nn & 1);
    return t * t;
}
