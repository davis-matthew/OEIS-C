/*
 * A220337  A modified Engel expansion for 3*sqrt(15) - 11.
 * https://oeis.org/A220337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows * a(1) = 2, a(2) = 5, a(3) = 8, * a(3k+1) = 2, *
 *            a(3k+2) = 2 * (a(3k-1)^2 - 2*a(3k-1) + 1), * a(3k+3) = 2*a(3k+2)
 *            - 2 (for k ≥ 0). * This implements the recurrence directly
 *            without external libraries
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 13, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * 
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0 */

    /* Initial three terms: positions 1,2,3 map to indices 1,2,0 respectively */
    seq_t buf[3];
    buf[1] = (seq_t)2;  /* a(1) */
    buf[2] = (seq_t)5;  /* a(2) */
    buf[0] = (seq_t)8;  /* a(3) */

    /* Compute terms up to n */
    for (idx_t i = 4; i <= n; ++i)
    {
        int idx = (int)(i % 3);          /* 0,1,2 correspond to positions 3k,3k+1,3k+2 */
        seq_t val;

        if (idx == 1)                   /* a(3k+1) = 2 */
        {
            val = (seq_t)2;
        }
        else if (idx == 2)              /* a(3k+2) = 2*(prev^2 - 2*prev + 1) */
        {
            seq_t prev = buf[(int)((i - 3) % 3)];
            seq_t t = prev * prev - prev * (seq_t)2 + (seq_t)1;
            val = (seq_t)2 * t;
        }
        else                            /* idx == 0, a(3k+3) = 2*prev - 2 */
        {
            seq_t prev = buf[(int)((i - 1) % 3)];
            val = (seq_t)2 * prev - (seq_t)2;
        }

        buf[idx] = val;
    }

    return buf[(int)(n % 3)];
}
