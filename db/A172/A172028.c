/*
 * A172028  a(1) = 2; for n > 1, a(n) = smallest k such that a(n-1)^3+k is a
 *          cube.
 * https://oeis.org/A172028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 3*a(n-1)*(a(n-1)+1), a(1) = 2. This function computes
 *            the n-th term of OEIS A172028 by iteratively applying the
 *            recurrence. It works for all supported seq_t types (fixed‑width
 *            or arbitrary‑precision) and for any order of calls
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1; return 0 */
        return (seq_t)0;

    seq_t cur = (seq_t)2;   /* a(1) */
    if (n == 1)
        return cur;

    for (idx_t i = 2; i <= n; ++i)
        cur = (seq_t)1 + (seq_t)3 * cur * (cur + (seq_t)1);

    return cur;
}
