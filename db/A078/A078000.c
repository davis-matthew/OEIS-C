/*
 * A078000  Duplicate of A007909.
 * https://oeis.org/A078000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=a(1)=a(2)=1; for n >= 3, a(n) = 2*a(n-1) + c[n mod 4],
 *            where c = {+1, -1, -1, +1} (indices 0..3)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;

    const int c[4] = {1, -1, -1, 1};
    seq_t cur = 1;                     /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        int mod = (int)(i & 3u);        /* n mod 4 */
        cur = (cur << 1) + (seq_t)c[mod];
    }
    return cur;
}
