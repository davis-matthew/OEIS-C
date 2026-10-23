/*
 * A018620  Divisors of 740.
 * https://oeis.org/A018620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 740 in ascending order
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* comparison function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* we need only the divisors of 740 = 2^2 * 5 * 37 */
    static seq_t divisors[12];
    static int initialized = 0;

    if (!initialized)
    {
        idx_t pos = 0;
        for (idx_t e2 = 0; e2 <= 2; ++e2)
        {
            for (idx_t e5 = 0; e5 <= 1; ++e5)
            {
                for (idx_t e37 = 0; e37 <= 1; ++e37)
                {
                    seq_t d = ((seq_t)1 << e2);
                    d *= (e5 ? 5 : 1);
                    d *= (e37 ? 37 : 1);
                    divisors[pos++] = d;
                }
            }
        }
        /* sort the small list of 12 divisors */
        qsort(divisors, 12, sizeof(seq_t), cmp_seq);
        initialized = 1;
    }

    if (n == 0 || n > 12)   /* n outside the defined range */
        return 0;

    return divisors[n - 1];
}
