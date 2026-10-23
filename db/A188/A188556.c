/*
 * A188556  Number of 5 X n binary arrays without the pattern 0 1 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 - 5*n^4 + 45*n^3 + 5*n^2 + 314*n + 360) / 120
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a larger intermediate type to avoid overflow.
       The expression works for all n where the result fits
       in seq_t, including 64‑bit builds. */
#if defined(__SIZEOF_INT128__)
    typedef __int128 int128_t;
    int128_t nn = (int128_t)n;
    int128_t n2 = nn * nn;      /* n^2 */
    int128_t n3 = n2 * nn;      /* n^3 */
    int128_t n4 = n3 * nn;      /* n^4 */
    int128_t n5 = n4 * nn;      /* n^5 */

    int128_t t = n5               /* n^5 */
               + (int128_t)45 * n3   /* +45 n^3 */
               + (int128_t)5  * n2   /* +5  n^2 */
               + (int128_t)314 * nn  /* +314 n */
               + (int128_t)360;      /* +360 */

    int128_t s = (int128_t)5 * n4;   /* 5 n^4 */

    int128_t diff = t - s;          /* numerator */

    /* division is exact by construction */
    return (seq_t)(diff / 120);
#else
    /* Fallback for compilers without __int128 support:
       use the recurrence
       a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3)
              - 15*a(n-4) + 6*a(n-5) - a(n-6)   for n > 6   */
    static const seq_t init[6] = {6, 11, 20, 36, 64, 112};
    if (n <= 6) return init[n - 1];

    seq_t buf[6] = {init[0], init[1], init[2], init[3], init[4], init[5]};

    for (idx_t i = 7; i <= n; ++i) {
        seq_t sum_pos = 6 * buf[5] + 20 * buf[3] + 6 * buf[1];
        seq_t sum_neg = 15 * buf[4] + 15 * buf[2] + buf[0];
        seq_t next = sum_pos - sum_neg;

        /* rotate the buffer */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }
    return buf[5];
#endif
}
