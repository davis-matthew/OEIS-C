/*
 * A223859  Number of n X 3 0..3 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A223859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10) for n > 10; a(1)=50, a(2)=684, a(3)=4884, a(4)=24199,
 *            a(5)=93731, a(6)=303560, a(7)=857696, a(8)=2175884,
 *            a(9)=5058530, a(10)=10940664
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* ensure the cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    idx_t new_size = n + 1;                      /* indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);

    /* zero out the newly allocated part */
    for (idx_t i = cache_size + 1; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;

    /* compute missing values */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 10) {           /* base values */
            switch (i) {
                case 1: cache[i] = (seq_t)50;         break;
                case 2: cache[i] = (seq_t)684;        break;
                case 3: cache[i] = (seq_t)4884;       break;
                case 4: cache[i] = (seq_t)24199;      break;
                case 5: cache[i] = (seq_t)93731;      break;
                case 6: cache[i] = (seq_t)303560;     break;
                case 7: cache[i] = (seq_t)857696;     break;
                case 8: cache[i] = (seq_t)2175884;    break;
                case 9: cache[i] = (seq_t)5058530;    break;
                case 10: cache[i] = (seq_t)10940664;  break;
            }
        } else {                   /* recurrence for i > 10 */
            seq_t pos = (seq_t)0;
            seq_t sub = (seq_t)0;

            pos += cache[i-1] * (seq_t)10;
            pos += cache[i-3] * (seq_t)120;
            pos += cache[i-5] * (seq_t)252;
            pos += cache[i-7] * (seq_t)120;
            pos += cache[i-9] * (seq_t)10;

            sub += cache[i-2] * (seq_t)45;
            sub += cache[i-4] * (seq_t)210;
            sub += cache[i-6] * (seq_t)210;
            sub += cache[i-8] * (seq_t)45;
            sub += cache[i-10] * (seq_t)1;

            cache[i] = pos - sub;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, but guard against 0 */
    ensure_cache(n);
    return cache[n];
}
