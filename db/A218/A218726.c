/*
 * A218726  a(n) = (23^n - 1)/22.
 * https://oeis.org/A218726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 23*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Previous two terms of the sequence:
       a0 = a(i-2), a1 = a(i-1) */
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;

    /* Iteratively compute up to a(n) using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = ((seq_t)24) * a1 - ((seq_t)23) * a0;
        a0 = a1;
        a1 = temp;
    }

    return a1;
}
