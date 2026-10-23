/*
 * A018708  Divisors of 896.
 * https://oeis.org/A018708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 896 in ascending order */
#include <stdlib.h>

/* comparison function for qsort */
static int cmp_seq_t(const void *p, const void *q)
{
    seq_t x = *(const seq_t *)p;
    seq_t y = *(const seq_t *)q;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Returns the n-th divisor of 896, with a(1)=1.  If n>16, 0 is returned. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1 */

    static seq_t divisors[16];
    static int initialized = 0;

    if (!initialized)
    {
        /* generate all divisors 2^i * 7^j, i=0..7, j=0..1 */
        idx_t k = 0;
        for (idx_t i = 0; i <= 7; ++i)
        {
            seq_t val = (seq_t)(1ULL << i);   /* 2^i */
            for (idx_t j = 0; j <= 1; ++j)
            {
                seq_t d = val;
                if (j) d *= (seq_t)7;          /* times 7 if j==1 */
                divisors[k++] = d;
            }
        }
        /* sort the list */
        qsort(divisors, 16, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }

    if (n <= 16) return divisors[n - 1];
    return 0;   /* undefined beyond known terms */
}
