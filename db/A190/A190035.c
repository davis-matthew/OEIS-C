/*
 * A190035  Number of nondecreasing arrangements of n+2 numbers in 0..3 with
 *          the last equal to 3 and each after the second equal to the sum of
 *          one or two of the preceding three.
 * https://oeis.org/A190035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 for n = 1, * a(n) = 7 for n = 2, * a(n) = 4*n - 2 for n
 *            > 2 (recurrence: a(n) = 2*a(n-1) - a(n-2) for n > 4)
 * Range:     u8 n <= 64, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)7;
    /* for n > 2 the closed‑form linear formula holds */
    return (seq_t)4 * (seq_t)n - (seq_t)2;
}
