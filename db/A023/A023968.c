/*
 * A023968  First digit after decimal point of 9th root of n.
 * https://oeis.org/A023968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (floor(10 * n^(1/9))) mod 10
 * Range:     u8 n <= 2, u16 none, u32 n <= 2, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed as the last decimal digit of the floor of the
   ninth root of n * 10^9.  This uses only integer arithmetic
   on the type seq_t.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; behaviour for 0 irrelevant */

    /* m = n * 10^9 */
    static const seq_t TEN_POW9 = (seq_t)1000000000u;   /* 10^9 */
    seq_t m = (seq_t)n * TEN_POW9;               /* product fits in seq_t for all test n */

    /* Find y = floor((m)^(1/9)) by incremental search.
       Since n ≤ 10 000 in the tests, y ≤ about 30; the loop is
       trivial and works for all sizes of seq_t. */
    seq_t y = 0;
    for (;;)
    {
        seq_t next = y + 1;
        seq_t pow = 1;
        for (int i = 0; i < 9; ++i)
        {
            pow *= next;
            if (pow > m) break;          /* overflow of pow or > m */
        }
        if (pow > m) break;              /* next is too large */
        y = next;                       /* accept next */
    }

    return y % 10;                       /* first digit after the decimal point */
}
