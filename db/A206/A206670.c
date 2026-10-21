/*
 * A206670  Number of (n+1) X 3 0..2 arrays with no 2 X 2 subblock having the
 *          same number of clockwise edge increases as its horizontal
 *          neighbors or the same number of counterclockwise edge increases as
 *          its vertical neighbors.
 * https://oeis.org/A206670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            4a(n‑1)+22a(n‑2)−59a(n‑3)−112a(n‑4)−136a(n‑5)+525a(n‑6)+182a(n‑7)−684a(n‑8)
 *            +417a(n‑9)+136a(n‑10)+252a(n‑11)+514a(n‑12)+224a(n‑13)+52a(n‑14)−80a(n‑15)−48a(n‑16)
 *            Initial values: a(1)=360, a(2)=1812, a(3)=9318, a(4)=47106,
 *            a(5)=243432, a(6)=1242930, a(7)=...
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init_terms[16] = {
    (seq_t)360, (seq_t)1812, (seq_t)9318, (seq_t)47106, (seq_t)243432,
    (seq_t)1242930, (seq_t)6474150, (seq_t)33323136, (seq_t)173699544,
    (seq_t)898948062, (seq_t)4687777926, (seq_t)24335901672,
    (seq_t)126930698646, (seq_t)660162473754, (seq_t)3443416243986,
    (seq_t)17928893882562
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static const int coeff[16] = {4, 22, -59, -112, -136, 525, 182, -684,
                              417, 136, 252, 514, 224, 52, -80, -48};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n=0 */

    /* initialize cache with known terms once */
    if (cache == NULL) {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        memcpy(cache, init_terms, 16 * sizeof(seq_t));
        cache_size = 16;
    }

    /* extend cache if necessary */
    if (n > cache_size) {
        /* allocate space for n terms */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;

        /* compute terms from current size up to n-1 (zero‑based index) */
        for (idx_t k = cache_size; k < n; ++k) {
            /* terms 1..16 are already in cache */
            if (k < 16) continue;

            seq_t sum = 0;
            for (int i = 0; i < 16; ++i) {
                seq_t mult = cache[k - i - 1] * (seq_t)coeff[i];
                sum += mult;
            }
            cache[k] = sum;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
