/*
 * A036656  Number of n-node rooted unlabeled trees with outdegree <= 2 and
 *          exactly 1 edge at the root.
 * https://oeis.org/A036656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A036656
   a(n) = 0, n<2
   For n≥2, let t[0]=0, t[1]=1 and for m≥2
        t[m] = t[m-1] + ( Σ_{i=1}^{m-1} t[i]*t[m-1-i] +
                          (m-1 even ? t[(m-1)/2] : 0) ) / 2
   Then a(n) = t[n-1].
   This counts n‑node rooted unlabeled trees with outdegree ≤2
   whose root has exactly one child. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t need = n - 1;               /* index of t that is needed */
    static seq_t *t = NULL;           /* array of t[0..max] */
    static idx_t max = 0;             /* largest computed index */

    if (need > max) {
        /* allocate space for t[0..need] */
        size_t newsize = (size_t)(need + 1);
        seq_t *tmp = (seq_t *)malloc(newsize * sizeof(seq_t));
        if (!tmp)
            exit(1);                  /* allocation failed */
        if (t) {
            memcpy(tmp, t, (size_t)(max + 1) * sizeof(seq_t));
            free(t);
        }
        t = tmp;
        /* initialise the first computed entry if necessary */
        if (max < 1) {
            t[0] = 0;
            t[1] = 1;
            max = 1;
        }
        /* compute t from max+1 up to need */
        for (idx_t m = max + 1; m <= need; ++m) {
            seq_t sum = 0;
            for (idx_t i = 1; i < m; ++i) {
                sum += t[i] * t[m - 1 - i];
            }
            seq_t extra = 0;
            if (((m - 1) & 1) == 0)          /* m-1 even */
                extra = t[(m - 1) / 2];
            t[m] = t[m - 1] + (sum + extra) / 2;
        }
        max = need;
    }
    return t[need];
}
