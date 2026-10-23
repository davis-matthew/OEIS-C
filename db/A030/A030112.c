/*
 * A030112  Number of antichains in the product of a fence poset with n
 *          elements and a chain with 7 elements; also number of paths with n
 *          turns when light is reflected from 8 glass plates.
 * https://oeis.org/A030112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A030112: a(n) = 4*a(n-1)+10*a(n-2)-10*a(n-3)-15*a(n-4)+6*a(n-5)+7*a(n-6)-a(n-7)-a(n-8)
   Initial values: a(0)=1, a(1)=8, a(2)=36, a(3)=204, a(4)=1086, a(5)=5916, a(6)=31998, a(7)=173502
*/
seq_t a(idx_t n)
{
    /* base cases */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)8, (seq_t)36, (seq_t)204,
        (seq_t)1086, (seq_t)5916, (seq_t)31998, (seq_t)173502
    };
    if (n <= 7) {
        return init[n];
    }

    /* sliding window of the last eight values */
    seq_t v[8];
    for (int i = 0; i < 8; ++i) {
        v[i] = init[i];
    }

    for (idx_t k = 8; k <= n; ++k) {
        /* compute next term using the recurrence */
        seq_t sum1 = v[7] * (seq_t)4 + v[6] * (seq_t)10 + v[3] * (seq_t)6 + v[2] * (seq_t)7;
        seq_t sum2 = v[5] * (seq_t)10 + v[4] * (seq_t)15 + v[1] * (seq_t)1 + v[0] * (seq_t)1;
        seq_t next = sum1 - sum2;

        /* shift the window */
        for (int i = 0; i < 7; ++i) {
            v[i] = v[i + 1];
        }
        v[7] = next;
    }

    return v[7];
}
