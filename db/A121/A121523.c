/*
 * A121523  Number of up steps starting at an even level in all nondecreasing
 *          Dyck paths of semilength n. A nondecreasing Dyck path is a Dyck
 *          path for which the sequence of the altitudes of the valleys is
 *          nondecreasing.
 * https://oeis.org/A121523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6*a(n-1) - 9*a(n-2) - 5*a(n-3) + 15*a(n-4)
 *              - a(n-5) - 4*a(n-6) + a(n-7)  + p(n)
 * p(1)= 1, p(2)= -3, p(3)= 1, p(4)= 5, p(5)= -5, p(n)=0 for n>5.
 * (Recurrence obtained from the rational generating function in OEIS.)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t sz  = 0;          /* number of elements allocated (>= n+1) */

    if (n == 0) return 0;          /* value for offset < 1 – any value acceptable */

    /* grow array if needed */
    if (sz <= n) {
        idx_t newsz = n + 1;
        if (seq == NULL)
            seq = (seq_t *)realloc(NULL, newsz * sizeof(seq_t));
        else
            seq = (seq_t *)realloc(seq, newsz * sizeof(seq_t));

        /* initialise new part */
        for (idx_t i = sz; i < newsz; ++i) seq[i] = 0;
        seq[0] = 0;               /* base case */

        /* compute values from first missing index up to n */
        idx_t start = (sz > 0) ? sz : 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = 0;

            if (i >= 1) val += 6 * seq[i-1];
            if (i >= 2) val -= 9 * seq[i-2];
            if (i >= 3) val -= 5 * seq[i-3];
            if (i >= 4) val += 15 * seq[i-4];
            if (i >= 5) val -= seq[i-5];
            if (i >= 6) val -= 4 * seq[i-6];
            if (i >= 7) val += seq[i-7];

            /* add the p(i) term for i <= 5 */
            switch (i) {
                case 1: val += 1;  break;
                case 2: val -= 3;  break;
                case 3: val += 1;  break;
                case 4: val += 5;  break;
                case 5: val -= 5;  break;
                default: break;
            }
            seq[i] = val;
        }
        sz = newsz;
    }
    return seq[n];
}
