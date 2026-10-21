/*
 * A229442  Number of n X 5 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, and rows and columns
 *          lexicographically nondecreasing.
 * https://oeis.org/A229442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula used: linear recurrence of order 11  
 * a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) + 462*a(n-5)
 *        - 462*a(n-6) + 330*a(n-7) - 165*a(n-8) + 55*a(n-9)
 *        - 11*a(n-10) + a(n-11)      for n > 11
 * Initial values: a(1)=7, a(2)=19, a(3)=60, a(4)=212, a(5)=753,
 *                a(6)=2546, a(7)=8024, a(8)=23428, a(9)=63430,
 *                a(10)=159945, a(11)=377840
 */

#include <stddef.h>

static const seq_t init_vals[11] = {
    (seq_t)7, (seq_t)19, (seq_t)60, (seq_t)212, (seq_t)753,
    (seq_t)2546, (seq_t)8024, (seq_t)23428, (seq_t)63430,
    (seq_t)159945, (seq_t)377840
};

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0; /* undefined, but safe guard */

    if (n <= 11)
        return init_vals[n - 1];

    /* buffer holds the last 11 values: last[0]=a(n-11), ..., last[10]=a(n-1) */
    seq_t last[11];
    for (idx_t i = 0; i < 11; ++i)
        last[i] = init_vals[i];

    idx_t cur_n = 12;
    for (; cur_n <= n; ++cur_n) {
        seq_t next =
            (seq_t)11   * last[10] +
            (seq_t)-55  * last[9]  +
            (seq_t)165  * last[8]  +
            (seq_t)-330 * last[7]  +
            (seq_t)462  * last[6]  +
            (seq_t)-462 * last[5]  +
            (seq_t)330  * last[4]  +
            (seq_t)-165 * last[3]  +
            (seq_t)55   * last[2]  +
            (seq_t)-11  * last[1]  +
            (seq_t)1    * last[0];

        /* shift the window */
        for (idx_t i = 0; i < 10; ++i)
            last[i] = last[i + 1];
        last[10] = next;
    }

    /* cur_n == n+1 now, last[10] holds a(n) */
    return last[10];
}
