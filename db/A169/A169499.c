/*
 * A169499  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 , n = 0 6 * 5^(n-1) , 1 ≤ n ≤ 33 4 * (a(n-1)+ … +
 *            a(n-33)) – 10 * a(n-34) , n ≥ 34 where a(n) denotes the number
 *            of reduced words of length n in the Coxeter group on 6
 *            generators with relations (S_i)^2 = (S_i S_j)^3 = I
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;
    if (n <= 33) {
        seq_t v = (seq_t)6;
        for (idx_t i = 2; i <= n; ++i)
            v = v * (seq_t)5;
        return v;
    }

    /* For n ≥ 34 use a circular buffer of the last 34 terms */
    seq_t buf[34];
    /* Initialise a[0] … a[33] */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)6;
    for (idx_t i = 2; i <= 33; ++i)
        buf[i] = buf[i-1] * (seq_t)5;

    /* Maintain the sum of the last 33 values (indices 1 … 33) */
    seq_t sum = (seq_t)0;
    for (idx_t j = 1; j <= 33; ++j)
        sum += buf[j];

    /* Iterate from n = 34 up to the requested n */
    for (idx_t k = 34; k <= n; ++k) {
        seq_t next = (seq_t)((seq_t)4 * sum - (seq_t)10 * buf[(k-34) % 34]);

        /* Update the sliding window sum */
        sum   += next;
        sum   -= buf[(k-33) % 34];

        /* Store the new value in the circular buffer */
        buf[k % 34] = next;
    }

    return buf[n % 34];
}
