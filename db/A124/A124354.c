/*
 * A124354  Orders of the automorphisms groups of the n-antiprism graph.
 * https://oeis.org/A124354
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3) = 48, a(n) = 4*n for all n ≠ 3 (n ≥ 3)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This satisfies both the explicit formula and the recurrence
   a(n) = 2*a(n-1) - a(n-2) for n > 5. */

seq_t a(idx_t n)
{
    if (n == 3)
        return 48;
    /* For n ≥ 4, a(n) = 4*n */
    return (seq_t)(4 * n);
}
