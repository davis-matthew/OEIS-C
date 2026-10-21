/*
 * A086419  Sum of all prime factors of 3-smooth numbers.
 * https://oeis.org/A086419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of prime factors of the n‑th 3‑smooth number
 *            (A003586)
 * Range:     u8 n <= 5272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static const long double LOG2_VAL = log((long double)2);
static const long double LOG3_VAL = log((long double)3);

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, but keep safe   */

    /* Allocate arrays for exponents of 2 and 3, and for the sums */
    idx_t *e2 = (idx_t*)malloc(sizeof(idx_t) * (n + 1));
    idx_t *e3 = (idx_t*)malloc(sizeof(idx_t) * (n + 1));
    seq_t *s  = (seq_t*)malloc(sizeof(seq_t) * (n + 1));

    /* Base case: the first 3‑smooth number is 1 */
    e2[1] = 0;   e3[1] = 0;
    s[1]  = 0;   /* sum of prime factors of 1 is 0 */

    if (n > 1) {
        idx_t idx2 = 1, idx3 = 1;          /* next candidates from multiplying by 2 or 3 */

        for (idx_t k = 2; k <= n; ++k) {
            /* Detect duplicate value when 2*value[idx2] == 3*value[idx3] */
            if (e2[idx2] + 1 == e2[idx3] && e3[idx2] == e3[idx3] + 1) {
                e2[k] = e2[idx2] + 1;
                e3[k] = e3[idx2];
                s[k]  = s[idx2] + 2;        /* or s[idx3] + 3; both equal */
                ++idx2; ++idx3;
            } else {
                /* Compare 2^(e2+1) * 3^e3  vs  2^e2 * 3^(e3+1) using logarithms */
                long double val2 = (long double)(e2[idx2] + 1) * LOG2_VAL
                                 + (long double)e3[idx2]          * LOG3_VAL;
                long double val3 = (long double)e2[idx3]          * LOG2_VAL
                                 + (long double)(e3[idx3] + 1) * LOG3_VAL;

                if (val2 < val3) {                 /* pick the smaller candidate */
                    e2[k] = e2[idx2] + 1;
                    e3[k] = e3[idx2];
                    s[k]  = s[idx2] + 2;
                    ++idx2;
                } else {
                    e2[k] = e2[idx3];
                    e3[k] = e3[idx3] + 1;
                    s[k]  = s[idx3] + 3;
                    ++idx3;
                }
            }
        }
    }

    seq_t result = s[n];
    free(e2); free(e3); free(s);
    return result;
}
