/*
 * A239450  Numbers m such that T(m)^2 + T(m^2) is a perfect square, where T =
 *          A000217.
 * https://oeis.org/A239450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=3, a(3)=47, a(4)=659; for n>4:
 *            a(n)=15*(a(n-1)-a(n-2))+a(n-3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly */
    if (n <= 1) return 0;          /* safety for n=0, though OEIS offset is 1 */
    if (n == 2) return 3;
    if (n == 3) return 47;
    if (n == 4) return 659;

    /* For n > 4 iterate using the recurrence */
    seq_t prev3 = 3;    /* a(2) */
    seq_t prev2 = 47;   /* a(3) */
    seq_t prev1 = 659;  /* a(4) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = 15 * (prev1 - prev2) + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
