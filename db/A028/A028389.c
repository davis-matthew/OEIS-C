/*
 * A028389  The 5x + 1 sequence beginning at 7.
 * https://oeis.org/A028389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 35, u32 n <= 188, u64 n <= 734,
 *            u128 n <= 1135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A028389:
   a(0) = 7;
   a(n) = 5*a(n-1)+1 if a(n-1) is odd,
          a(n-1)/2 if a(n-1) is even. */

seq_t a(idx_t n)
{
    seq_t val = 7;
    for (idx_t i = 0; i < n; ++i)
    {
        if (val % 2 != 0)          /* odd */
            val = 5 * val + 1;
        else                       /* even */
            val = val / 2;
    }
    return val;
}
