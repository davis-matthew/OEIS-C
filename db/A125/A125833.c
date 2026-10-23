/*
 * A125833  Numbers whose base-5 representation is 333333.......3.
 * https://oeis.org/A125833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(5^n - 1)/4. Recurrence: a(0) = 0; a(n) = 5*a(n-1) + 3
 *            for n > 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic and works for any
   seq_t chosen by the build.  It is O(n) and does not require the
   external power5 helper. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t res = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        res = (seq_t)((seq_t)5 * res + (seq_t)3);
    }
    return res;
}
