/*
 * A181405  Total number of n-digit numbers requiring 8 positive cubes in
 *          their representation as sum of cubes.
 * https://oeis.org/A181405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3 if n==2, 12 if n==3, otherwise 0. Explanation: Only 15
 *            positive integers require exactly eight positive cubes. Three
 *            are two‑digit numbers and twelve are three‑digit numbers; all
 *            other n‑digit numbers need fewer or more than eight cubes
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)12;
    return (seq_t)0;
}
