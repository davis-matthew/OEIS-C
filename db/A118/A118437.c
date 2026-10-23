/*
 * A118437  Row sums of triangle A118435.
 * https://oeis.org/A118437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 21, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A118437 (row sums of triangle A118435):
   a(0)=1, a(1)=2, a(2)=0, a(3)=8, a(4)=-32, a(5)=-128,
   a(6)=320, a(7)=128
   For n ≥ 8:
   a(n) = 2*a(n-1) -4*a(n-2) +8*a(n-3) -64*a(n-4) +
          128*a(n-5) -256*a(n-6) +512*a(n-7)   */
seq_t a(idx_t n)
{
    /* initial values a(0) … a(7) */
    static const seq_t init[8] = {
        (seq_t)1,  (seq_t)2,  (seq_t)0,  (seq_t)8,
        (seq_t)-32, (seq_t)-128, (seq_t)320, (seq_t)128
    };

    if (n < 8)
        return init[n];

    /* last[0] … last[6] will hold a_{k-7} … a_{k-1} during the loop
       For the first iteration with k = 8 we need a(1)…a(7) */
    seq_t last[7];
    last[0] = init[1];
    last[1] = init[2];
    last[2] = init[3];
    last[3] = init[4];
    last[4] = init[5];
    last[5] = init[6];
    last[6] = init[7];

    seq_t next;
    for (idx_t k = 8; k <= n; ++k) {
        __int128 s = (__int128)512 * (__int128)last[0]
                    - (__int128)256 * (__int128)last[1]
                    + (__int128)128 * (__int128)last[2]
                    - (__int128)64  * (__int128)last[3]
                    + (__int128)8   * (__int128)last[4]
                    - (__int128)4   * (__int128)last[5]
                    + (__int128)2   * (__int128)last[6];
        next = (seq_t)s;

        /* shift window for next k */
        for (int i = 0; i < 6; ++i)
            last[i] = last[i + 1];
        last[6] = next;
    }
    return last[6];
}
