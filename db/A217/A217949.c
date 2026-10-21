/*
 * A217949  Number of n-element 0..3 arrays with each element the minimum of 3
 *          adjacent elements of a random 0..3 array of n+2 elements.
 * https://oeis.org/A217949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence * a(n) = 4*a(n-1) - 6*a(n-2)
 *            + 4*a(n-3) + 5*a(n-4) * - 4*a(n-5) + 6*a(n-6) + 4*a(n-7) +
 *            2*a(n-9) + a(n-10) * with initial values a(1)=4, a(2)=16,
 *            a(3)=50, a(4)=130, * a(5)=310, a(6)=736, a(7)=1821, a(8)=4673,
 *            a(9)=12107, a(10)=31103
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        (seq_t)4,
        (seq_t)16,
        (seq_t)50,
        (seq_t)130,
        (seq_t)310,
        (seq_t)736,
        (seq_t)1821,
        (seq_t)4673,
        (seq_t)12107,
        (seq_t)31103
    };

    if (n <= 10)
        return init[n - 1];

    /* last[0] = a(n-1), last[1] = a(n-2), ..., last[9] = a(n-10) */
    seq_t last[10];
    /* initialise for n = 11 */
    last[0] = init[9]; /* a10 */
    last[1] = init[8]; /* a9  */
    last[2] = init[7]; /* a8  */
    last[3] = init[6]; /* a7  */
    last[4] = init[5]; /* a6  */
    last[5] = init[4]; /* a5  */
    last[6] = init[3]; /* a4  */
    last[7] = init[2]; /* a3  */
    last[8] = init[1]; /* a2  */
    last[9] = init[0]; /* a1  */

    for (idx_t i = 11; i <= n; ++i)
    {
        /* compute a(i) using the recurrence */
        seq_t sum_pos = (seq_t)0;
        sum_pos += (seq_t)4 * last[0];   /* 4*a(n-1)    */
        sum_pos += (seq_t)4 * last[2];   /* 4*a(n-3)    */
        sum_pos += (seq_t)5 * last[3];   /* 5*a(n-4)    */
        sum_pos += (seq_t)6 * last[5];   /* 6*a(n-6)    */
        sum_pos += (seq_t)4 * last[6];   /* 4*a(n-7)    */
        sum_pos += (seq_t)2 * last[8];   /* 2*a(n-9)    */
        sum_pos += last[9];              /*   a(n-10)   */

        seq_t sum_neg = (seq_t)0;
        sum_neg += (seq_t)6 * last[1];   /* 6*a(n-2)    */
        sum_neg += (seq_t)4 * last[4];   /* 4*a(n-5)    */

        seq_t new_val = sum_pos - sum_neg;

        /* shift the window: last[9] <- last[8] ... last[1] <- last[0] <- new_val */
        for (idx_t k = 9; k > 0; --k)
            last[k] = last[k - 1];
        last[0] = new_val;
    }

    return last[0];
}
