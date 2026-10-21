/*
 * A223904  Poly-Cauchy numbers of the second kind hat c_n^(-5).
 * https://oeis.org/A223904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (k+1)^5 * Stirling1(n,k)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 9, i64 n <= 17, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *c_table = NULL;          /* table of unsigned Stirling numbers of the first kind */
static idx_t pre_max = 0;              /* highest n computed (n values 0..pre_max-1 present) */

seq_t a(idx_t n)
{
    /* Ensure table contains values up to n+1 */
    idx_t target = n + 1;
    if (target >= pre_max)
    {
        idx_t new_max = target + 1;                 /* compute up to target */
        seq_t *new_tbl = (seq_t *)realloc(c_table, new_max * 7 * sizeof(seq_t));
        if (!new_tbl) exit(1);                      /* memory error */

        /* Initialize newly allocated entries to zero */
        for (idx_t i = pre_max * 7; i < new_max * 7; ++i)
            new_tbl[i] = 0;

        /* Base case: c(0,0) = 1 */
        if (pre_max == 0)
            new_tbl[0] = 1;

        /* Recurrence for unsigned Stirling numbers up to new_max-1 */
        for (idx_t i = 1; i <= new_max - 1; ++i)
        {
            new_tbl[i * 7 + 0] = 0;                 /* c(i,0) = 0 for i > 0 */
            for (int k = 1; k <= 6; ++k)
            {
                seq_t a1 = (k-1 <= i-1) ? new_tbl[(i-1) * 7 + (k-1)] : 0;
                seq_t a2 = (k   <= i-1) ? new_tbl[(i-1) * 7 +  k   ] : 0;
                new_tbl[i * 7 + k] = a1 + (seq_t)(i-1) * a2;
            }
        }

        c_table = new_tbl;
        pre_max = new_max;
    }

    /* Weights derived from k! * Stirling2(6,k+1) for k=0..5 */
    static const seq_t weights[6] = {1, 31, 180, 390, 360, 120};

    /* Compute sum of weighted Stirling numbers for n+1 */
    seq_t sum = 0;
    for (int k = 1; k <= 6; ++k)
        sum += weights[k-1] * c_table[(n + 1) * 7 + k];

    /* Apply outer sign (-1)^n */
    if (n & 1)
        sum = -sum;

    return sum;
}
