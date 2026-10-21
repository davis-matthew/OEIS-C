/*
 * A217953  Number of n element 0..3 arrays with each element the minimum of 7
 *          adjacent elements of a random 0..3 array of n+6 elements.
 * https://oeis.org/A217953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the empirical recurrence a(n) = 4*a(n-1) -
 *            6*a(n-2) + 4*a(n-3) - a(n-4) + 6*a(n-8) - 4*a(n-9) + 6*a(n-10) +
 *            4*a(n-11) + 5*a(n-12) + 6*a(n-13) + 7*a(n-14) + 8*a(n-15) +
 *            4*a(n-16) + 6*a(n-17) + 5*a(n-18) + 4*a(n-19) + 3*a(n-20) +
 *            2*a(n-21) + a(n-22) with the initial 22 terms a(1)=4...
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 31, u64 n <= 66, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial values */
static const seq_t init_vals[22] = {
    (seq_t)4,    (seq_t)16,   (seq_t)50,   (seq_t)130,
    (seq_t)296,  (seq_t)610,  (seq_t)1163, (seq_t)2083,
    (seq_t)3558, (seq_t)5902, (seq_t)9734, (seq_t)16410,
    (seq_t)28969,(seq_t)54049,(seq_t)105529,(seq_t)211097,
    (seq_t)423650,(seq_t)840654,(seq_t)1636964,(seq_t)3121556,
    (seq_t)5839194,(seq_t)10760322
};

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static idx_t cache_size = 0;

/*  Return the nth term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undefined, but return 0 */

    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size; i < n; ++i) {
            idx_t pos = i + 1;   /* sequence index (1‑based) */
            seq_t val;
            if (pos <= 22) {
                val = init_vals[pos - 1];
            } else {
                /* apply recurrence */
                seq_t v = 0;
                v += (seq_t)4 * cache[pos - 2];
                v -= (seq_t)6 * cache[pos - 3];
                v += (seq_t)4 * cache[pos - 4];
                v -= cache[pos - 5];
                v += (seq_t)6 * cache[pos - 9];
                v -= (seq_t)4 * cache[pos - 10];
                v += (seq_t)6 * cache[pos - 11];
                v += (seq_t)4 * cache[pos - 12];
                v += (seq_t)5 * cache[pos - 13];
                v += (seq_t)6 * cache[pos - 14];
                v += (seq_t)7 * cache[pos - 15];
                v += (seq_t)8 * cache[pos - 16];
                v += (seq_t)4 * cache[pos - 17];
                v += (seq_t)6 * cache[pos - 18];
                v += (seq_t)5 * cache[pos - 19];
                v += (seq_t)4 * cache[pos - 20];
                v += (seq_t)3 * cache[pos - 21];
                v += (seq_t)2 * cache[pos - 22];
                v += cache[pos - 23];
                val = v;
            }
            cache[i] = val;
        }
        cache_size = n;
    }
    return cache[n - 1];
}
