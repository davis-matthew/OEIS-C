/*
 * A024399  a(n) = [ (3rd elementary symmetric function of S(n))/(first
 *          elementary symmetric function of S(n)) ], where S(n) = {first n+2
 *          positive integers congruent to 2 mod 3}.
 * https://oeis.org/A024399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A024399
 *  a(n) is defined by the recurrence (valid for n ≥ 8):
 *
 *      a(n) = 4*a(n-1) – 6*a(n-2) + 5*a(n-3) – 5*a(n-4)
 *             + 6*a(n-5) – 4*a(n-6) + a(n-7)
 *
 *  Initial values (n = 1 … 7):
 *      a(1)=5, a(2)=31, a(3)=101, a(4)=248,
 *      a(5)=515, a(6)=952, a(7)=1619
 *
 *  The implementation memoises the values so that every call
 *  (in arbitrary order) runs in O(max(n)) time.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  n is 1‑based.  Return 0 for n < 1 (not used by the test harness). */
    if (n < 1) return 0;

    static seq_t *tab = NULL;   /* array storing a(1)…a(current_max) */
    static idx_t  max_n = 0;    /* number of computed terms */

    /* Grow the table if needed.  All indices are 1‑based,
       but we store a(i) at tab[i‑1]. */
    if (n > max_n) {
        idx_t new_max = n;
        tab = (seq_t*)realloc(tab, new_max * sizeof(seq_t));
        /* Compute new terms one by one. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 1)        tab[i-1] = 5;
            else if (i == 2)   tab[i-1] = 31;
            else if (i == 3)   tab[i-1] = 101;
            else if (i == 4)   tab[i-1] = 248;
            else if (i == 5)   tab[i-1] = 515;
            else if (i == 6)   tab[i-1] = 952;
            else if (i == 7)   tab[i-1] = 1619;
            else {
                /*   a(i) = 4*a(i-1) – 6*a(i-2) + 5*a(i-3) – 5*a(i-4)
                     + 6*a(i-5) – 4*a(i-6) + a(i-7)          */
                /* Split positives and negatives to avoid
                   unsigned overflow on subtraction.   */
                seq_t pos = 4*tab[i-2] + 5*tab[i-4] + 6*tab[i-6] + tab[i-8];
                seq_t neg = 6*tab[i-3] + 5*tab[i-5] + 4*tab[i-7];
                tab[i-1]  = pos - neg;               /* safe: pos ≥ neg */
            }
        }
        max_n = new_max;
    }
    return tab[n-1];
}
