/*
 * A018305  Divisors of 150.
 * https://oeis.org/A018305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 150 in increasing order
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* static storage for the 12 divisors of 150 */
    static seq_t divs[12];
    static int count = 0;

    /* compute the divisor list only once */
    if (count == 0)
    {
        seq_t temp[12];
        int tcount = 0;
        const seq_t N = (seq_t)150;

        for (int i = 1; (seq_t)i * (seq_t)i <= N; ++i)
        {
            if (N % (seq_t)i == (seq_t)0)
            {
                temp[tcount++] = (seq_t)i;
                seq_t q = N / (seq_t)i;
                if (q != (seq_t)i)
                    temp[tcount++] = q;
            }
        }

        /* simple insertion sort for the temp values */
        for (int i = 1; i < tcount; ++i)
        {
            seq_t key = temp[i];
            int j = i - 1;
            while (j >= 0 && temp[j] > key)
            {
                temp[j + 1] = temp[j];
                --j;
            }
            temp[j + 1] = key;
        }

        /* copy sorted values into the permanent array */
        for (int i = 0; i < tcount; ++i)
            divs[i] = temp[i];

        count = tcount;
    }

    /* return the requested divisor or 0 if out of range */
    if (n >= 1 && (idx_t)n <= (idx_t)count)
        return divs[(size_t)n - 1];
    return (seq_t)0;
}
