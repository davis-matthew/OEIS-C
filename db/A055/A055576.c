/*
 * A055576  Sum of digits of a(n)^5 is equal to a(n).
 * https://oeis.org/A055576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = integer m such that the sum of digits of m^5 equals m.
   Known solutions are 0,1,28,35,36,46; the sequence terminates after six terms.
   For n > 6 the function returns 0. */

seq_t a(idx_t n)
{
    /* List of all solutions m with sum_digits(m^5) == m (for m < 100). */
    static const seq_t sol[6] = {0, 1, 28, 35, 36, 46};

    if (n == 0) return 0;          /* offset is 1, n < 1 is undefined */
    if (n <= 6) return sol[n - 1];
    return 0;                      /* no more terms in the sequence */
}
