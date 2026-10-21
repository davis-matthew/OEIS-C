/*
 * A065548  a(1) = 1 and, for n > 0, a(2*n) = sum{a(i) | 0 < i <= n}, a(2*n+1)
 *          = a(n)^2.
 * https://oeis.org/A065548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>0: a(2*n)=∑_{i=1}^{n} a(i), a(2*n+1)=a(n)^2
 * Range:     u8 n <= 34, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_arr   = NULL;
    static seq_t *pref    = NULL;
    static size_t cur     = 0;   /* highest index computed */
    static size_t cap     = 0;   /* capacity of the arrays */

    if (n > cur) {
        /* enlarge arrays if necessary */
        size_t need = (size_t)n + 1;   /* indices 0..n need to be allocated */
        if (cap < need) {
            size_t newcap = (cap) ? cap << 1 : 16;
            while (newcap < need) newcap <<= 1;
            a_arr = (seq_t *)realloc(a_arr, newcap * sizeof(seq_t));
            pref  = (seq_t *)realloc(pref,  newcap * sizeof(seq_t));
            cap   = newcap;
        }

        /* compute values for indices cur+1 .. n */
        for (size_t i = cur + 1; i <= (size_t)n; ++i) {
            if (i == 1) {
                a_arr[1] = 1;
                pref[1]  = 1;
            } else if ((i & 1) == 0) {            /* even: a(2*n) = sum a(1..n) */
                seq_t sum = pref[i / 2];
                a_arr[i]  = sum;
                pref[i]   = pref[i - 1] + sum;
            } else {                               /* odd:  a(2*n+1) = a(n)^2 */
                seq_t v   = a_arr[(i - 1) / 2];
                seq_t sq  = v * v;
                a_arr[i]  = sq;
                pref[i]   = pref[i - 1] + sq;
            }
        }
        cur = (size_t)n;
    }
    return a_arr[(size_t)n];
}
