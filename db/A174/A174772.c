/*
 * A174772  y-values in the solution to x^2 - 33*y^2 = 1.
 * https://oeis.org/A174772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=4, a(n)=46*a(n-1)-a(n-2) for n>=3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For the first two indices return the constants directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    /* Compute the sequence iteratively up to n. */
    seq_t prev = (seq_t)0;      /* a(n-2) */
    seq_t curr = (seq_t)4;      /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)46 * curr - prev;  /* a(i) = 46*a(i-1)-a(i-2) */
        prev = curr;
        curr = next;
    }
    return curr;
}
