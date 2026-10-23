/*
 * A028345  Molien series for complete weight enumerator of self-dual code
 *          over GF(5) containing all-1's vector.
 * https://oeis.org/A028345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 419, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028345 – Molien series for complete weight enumerator of
   self‑dual code over GF(5) containing all‑1's vector.
   Recurrence (derived from the rational generating function):

   Initialize
      a[0] = 1,  a[1] = 1,  a[2] = 6,  a[3] = 21,  a[4] = 48,
      a[5] = 112, a[6] = 224, a[7] = 390, a[8] = 656, a[9] = 1037

   For n ≥ 10
      a[n] = 2*a[n‑1] – 3*a[n‑4] + 3*a[n‑6] – 2*a[n‑9] + a[n‑10]
*/

#include <stdlib.h>

static const seq_t init[10] = {
    1, 1, 6, 21, 48, 112, 224, 390, 656, 1037
};

seq_t a(idx_t n)
{
    /* static cache keeps all previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cached  = 0;      /* current number of stored terms */

    /* grow cache to contain terms 0...n */
    if (n + 1 > cached) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;

        for (idx_t i = cached; i < new_len; ++i) {
            if (i < 10) {
                cache[i] = init[i];
            } else {
                /* compute a[i] from the recurrence */
                seq_t pos = cache[i - 1] + cache[i - 1];        /* 2*a[i-1] */
                pos += cache[i - 6];
                pos += cache[i - 6];
                pos += cache[i - 6];                           /* +3*a[i-6] */
                pos += cache[i -10];                           /* +1*a[i-10] */

                seq_t neg = cache[i - 4];
                neg += cache[i - 4];
                neg += cache[i - 4];                           /* -3*a[i-4] */
                neg += cache[i - 9];
                neg += cache[i - 9];                           /* -2*a[i-9] */

                cache[i] = pos - neg;                          /* unsigned subtraction is safe */
            }
        }
        cached = new_len;
    }

    return cache[n];
}
