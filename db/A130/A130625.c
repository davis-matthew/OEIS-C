/*
 * A130625  First differences of A130624.
 * https://oeis.org/A130625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4, a(2) = 7 a(n) = 3*a(n-1) - 3*a(n-2) +
 *            2*a(n-3) (n >= 3)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) can also be written as a(n) = 2*a(n-1) + b(n)
   where b(n) is periodic with period 6:
     n mod 6 : 0  1  2   3   4   5
     b(n)    : 3  2 -1  -3  -2   1
   This form avoids any multiply by 3 or 2 that might overflow
   an unsigned seq_t when its value is still representable.  */

seq_t a(idx_t n)
{
    /* Small integer pattern for b(n)  (period 6) */
    static const int8_t pattern[6] = { 3, 2, -1, -3, -2, 1 };

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;

    /* Variables to hold the last three values */
    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)4;   /* a(n-2)   */
    seq_t a2 = (seq_t)7;   /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* cur = 2*a2 + b(i) */
        cur = a2 + a2;                      /* 2*a(n-1) */
        int8_t b = pattern[i % 6];           /* periodic offset */

        if (b >= 0)
            cur += (seq_t)b;                 /* add small positive */
        else
            cur -= (seq_t)(-b);              /* subtract small positive */

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return a2;
}
