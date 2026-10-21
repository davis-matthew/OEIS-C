/*
 * A226710  Number of n X 1 (-1,0,1) arrays of determinants of 2 X 2 subblocks
 *          of some (n+1) X 2 binary array.
 * https://oeis.org/A226710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) + a(n-4) + a(n-5)  
   with initial values a(1)=3, a(2)=9, a(3)=25, a(4)=67, a(5)=181.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, return 0 */
    static seq_t *buf = NULL;           /* buffer of computed terms (1‑based) */
    static idx_t last = 0;              /* index of largest computed term   */
    static idx_t cap  = 0;              /* allocated capacity (count of slots) */

    /* enlarge buffer if necessary (store up to n) */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure: return 0 */
        buf = tmp;
        cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) buf[i] = (seq_t)3;
        else if (i == 2) buf[i] = (seq_t)9;
        else if (i == 3) buf[i] = (seq_t)25;
        else if (i == 4) buf[i] = (seq_t)67;
        else if (i == 5) buf[i] = (seq_t)181;
        else {
            seq_t val = 2 * buf[i-1] + buf[i-2] + 2 * buf[i-3] + buf[i-4] + buf[i-5];
            buf[i] = val;
        }
    }
    last = n;
    return buf[n];
}
