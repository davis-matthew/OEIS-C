/*
 * A081219  One sixtieth the product of primitive Pythagorean triangles' sides
 *          whose odd values differ by 2.
 * https://oeis.org/A081219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(16*n^4 - 1)/15
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit integer for the intermediate calculations so that
       all products stay exact even when the final result fits in a
       smaller seq_t type.  All arithmetic is integer arithmetic only. */
    __int128 N = (unsigned long long)n;   /* promote idx_t to 128‑bit */
    __int128 prod = N * N;   /* n^2 */
    prod = prod * N;         /* n^3 */
    prod = prod * N;         /* n^4 */
    prod = prod * N;         /* n^5 */
    prod = prod * 16;        /* 16 * n^5 */
    prod -= N;               /* 16*n^5 - n */
    prod /= 15;              /* divide by 15 (exact) */
    return (seq_t)prod;
}
