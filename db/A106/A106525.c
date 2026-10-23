/*
 * A106525  Values of x in x^2 - 49 = 2*y^2.
 * https://oeis.org/A106525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 73, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-3) - a(n-6) for n >= 7
   Initial values: a(1)=9, a(2)=11, a(3)=21, a(4)=43, a(5)=57, a(6)=119
   (Offset = 1)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Unsigned types; value for n < 1 is irrelevant */
    if (n == 0) return (seq_t)0;

    static seq_t *buf  = NULL;   /* cache of computed terms (a(1) at buf[0]) */
    static idx_t  size = 0;      /* number of terms currently stored   */
    static idx_t  cap  = 0;      /* allocated capacity                 */

    /* Return cached value if we already have it */
    if (n <= size) return buf[n-1];

    /* Ensure we have enough space */
    if (n > cap) {
        idx_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < n) new_cap = n;          /* protect against overflow */
        buf = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t val;
        switch ((int)i) {
            case 1: val = (seq_t)9;   break;
            case 2: val = (seq_t)11;  break;
            case 3: val = (seq_t)21;  break;
            case 4: val = (seq_t)43;  break;
            case 5: val = (seq_t)57;  break;
            case 6: val = (seq_t)119; break;
            default:
                /* a(i) = 6*a(i-3) - a(i-6) */
                {
                    seq_t mul = buf[i-4] * (seq_t)6;
                    val = mul - buf[i-7];
                }
        }
        buf[i-1] = val;
    }

    size = n;
    return buf[n-1];
}
