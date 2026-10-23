/*
 * A186474  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 54, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A186474
    Formula: a(n) = 2*a(n-1) + 2*a(n-2) - 6*a(n-3) + 6*a(n-4) - 3*a(n-5)  for n > 12
    Initial values (1‑based index):
    a(1)=256, a(2)=205, a(3)=1098, a(4)=1251, a(5)=4895,
    a(6)=5955, a(7)=20079, a(8)=27101, a(9)=84228,
    a(10)=123227, a(11)=354915, a(12)=553287
*/

#include <stddef.h>

/* constant table for the first 12 terms (index 0 unused) */
static const seq_t init[13] = {
    (seq_t)0,
    (seq_t)256,   (seq_t)205,   (seq_t)1098,  (seq_t)1251,
    (seq_t)4895,  (seq_t)5955,  (seq_t)20079, (seq_t)27101,
    (seq_t)84228, (seq_t)123227,(seq_t)354915,(seq_t)553287
};

seq_t a(idx_t n)
{
    /* for the defined offset */
    if (n <= 12 && n >= 1)
        return init[(size_t)n];

    if (n == 0)   /* behaviour below the offset is irrelevant */
        return 0;

    /* initialise the five most recent values needed for the recurrence
       for the first time we compute a(13): a(8)..a(12) */
    seq_t prev[5];
    prev[0] = init[8];   /* a(8)   */
    prev[1] = init[9];   /* a(9)   */
    prev[2] = init[10];  /* a(10)  */
    prev[3] = init[11];  /* a(11)  */
    prev[4] = init[12];  /* a(12)  */

    seq_t val = 0;

    for (idx_t i = 13; i <= n; ++i) {
        val =   prev[4] * (seq_t)2
              + prev[3] * (seq_t)2
              - prev[2] * (seq_t)6
              + prev[1] * (seq_t)6
              - prev[0] * (seq_t)3;

        /* shift the window forward */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = val;
    }

    return val;
}
