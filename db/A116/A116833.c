/*
 * A116833  Number of permutations of length n which avoid the patterns 2134,
 *          3421, 4132.
 * https://oeis.org/A116833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=71, a(6)=213, a(7)=564,
 *            a(8)=1340 For n>8:
 *            a(n)=8*a(n‑1)-28*a(n‑2)+56*a(n‑3)-70*a(n‑4)+56*a(n‑5)-28*a(n‑6)+8*a(n‑7)-a(n‑8)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache the sequence up to the largest n requested so far */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms currently stored */

    if (n == 0)
        return 0;               /* undefined, but keep type consistent */

    if (cached < n) {
        size_t newsize = (size_t)n;
        seq_t *newcache = (seq_t *)malloc(newsize * sizeof(seq_t));
        if (newcache == NULL)
            return 0;          /* allocation failure – unlikely in tests */

        /* copy old values */
        for (size_t i = 0; i < cached; ++i)
            newcache[i] = cache[i];

        /* fill new entries */
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i <= 8) {            /* initial explicit values */
                switch (i) {
                case 1: newcache[0] = 1;          break;
                case 2: newcache[1] = 2;          break;
                case 3: newcache[2] = 6;          break;
                case 4: newcache[3] = 21;         break;
                case 5: newcache[4] = 71;         break;
                case 6: newcache[5] = 213;        break;
                case 7: newcache[6] = 564;        break;
                case 8: newcache[7] = 1340;       break;
                }
            } else {                       /* recurrence for i>8 */
                /* indices are 0‑based in cache */
                seq_t a1 = newcache[(size_t)(i-2)]; /* a(n-1) */
                seq_t a2 = newcache[(size_t)(i-3)]; /* a(n-2) */
                seq_t a3 = newcache[(size_t)(i-4)]; /* a(n-3) */
                seq_t a4 = newcache[(size_t)(i-5)]; /* a(n-4) */
                seq_t a5 = newcache[(size_t)(i-6)]; /* a(n-5) */
                seq_t a6 = newcache[(size_t)(i-7)]; /* a(n-6) */
                seq_t a7 = newcache[(size_t)(i-8)]; /* a(n-7) */
                seq_t a8 = newcache[(size_t)(i-9)]; /* a(n-8) */

                newcache[(size_t)(i-1)] =
                    8*a1 - 28*a2 + 56*a3 - 70*a4 + 56*a5 - 28*a6 + 8*a7 - a8;
            }
        }

        /* free old cache and adopt new one */
        if (cache != NULL)
            free(cache);
        cache  = newcache;
        cached = n;
    }

    return cache[(size_t)(n-1)];
}
