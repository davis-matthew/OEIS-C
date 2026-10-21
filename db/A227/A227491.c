/*
 * A227491  The hyper-Wiener index of the nanostar dendrimer D_n, defined
 *          pictorially in the Ghorbani et al. references and recursively in
 *          the Deutsch et al. reference.
 * https://oeis.org/A227491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 86*a(n-2) + 232*a(n-3) - 288*a(n-4) +
 *            128*a(n-5) for n > 5, with initial values: a(1)=1932,
 *            a(2)=305592, a(3)=6162360, a(4)=67702236, a(5)=555929988
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not part of the sequence, just a safe value */

    static seq_t *cache = NULL;   /* cache[0] corresponds to a(1) */
    static idx_t cap   = 0;       /* current size of cache */

    if (n > cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed – return 0 */
        cache = tmp;

        for (idx_t i = cap + 1; i <= n; ++i) {
            /* init base cases */
            if (i == 1) cache[0] = (seq_t)1932;
            else if (i == 2) cache[1] = (seq_t)305592;
            else if (i == 3) cache[2] = (seq_t)6162360;
            else if (i == 4) cache[3] = (seq_t)67702236;
            else if (i == 5) cache[4] = (seq_t)555929988;
            else { /* recurrence for i > 5 */
                seq_t t1 = cache[i-2];
                seq_t t2 = cache[i-3];
                seq_t t3 = cache[i-4];
                seq_t t4 = cache[i-5];
                seq_t t5 = cache[i-6];
                cache[i-1] = (seq_t)(15*t1 - 86*t2 + 232*t3 - 288*t4 + 128*t5);
            }
        }

        cap = n;
    }

    return cache[n-1];
}
