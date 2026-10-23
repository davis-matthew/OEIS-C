/*
 * A047881  a(n) = Sum_{h=0..n, k=0..n} T(h,k), array T counting knights'
 *          moves as in A049604.
 * https://oeis.org/A047881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A047881
      a(n) = a(n-1) + a(n-2) - 2*a(n-5) + a(n-8) + a(n-9) - a(n-10)
      a(0)=0, a(1)=10, a(2)=20, a(3)=38, a(4)=62, a(5)=100,
      a(6)=150, a(7)=220, a(8)=302, a(9)=410, a(10)=536, a(11)=692
*/
#include <stddef.h>   /* for size_t */

static const seq_t INIT[12] = {
    (seq_t)0, (seq_t)10, (seq_t)20, (seq_t)38,
    (seq_t)62, (seq_t)100, (seq_t)150, (seq_t)220,
    (seq_t)302, (seq_t)410, (seq_t)536, (seq_t)692
};

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 11)
        return INIT[(size_t)n];

    /* sliding window of the last 12 values (a(n-11) … a(n-1)) */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = INIT[i];

    /* compute iteratively up to n, reusing the buffer */
    for (idx_t i = 12; i <= n; ++i) {
        /* all indices are < i, so modulo by 12 yields a value <12 */
        seq_t t1  = buf[(i   - 1) % 12];
        seq_t t2  = buf[(i   - 2) % 12];
        seq_t t5  = buf[(i   - 5) % 12];
        seq_t t8  = buf[(i   - 8) % 12];
        seq_t t9  = buf[(i   - 9) % 12];
        seq_t t10 = buf[(i  - 10) % 12];

        seq_t term = t1 + t2 + t8 + t9;   /* a(i-1)+a(i-2)+a(i-8)+a(i-9) */
        term -= t5;                       /* subtract 1*a(i-5) */
        term -= t5;                       /* subtract another a(i-5) -> 2*a(i-5) */
        term -= t10;                      /* subtract a(i-10) */

        buf[i % 12] = term;                /* store a(i) */
    }

    return buf[n % 12];
}
