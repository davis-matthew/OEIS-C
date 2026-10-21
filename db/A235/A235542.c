/*
 * A235542  Number of (n+1)X(2+1) 0..1 arrays with the sum of each 2X2
 *          subblock two median terms lexicographically nondecreasing rowwise
 *          and columnwise.
 * https://oeis.org/A235542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n‑1) –48*a(n‑2) +26*a(n‑3) +319*a(n‑4) –694*a(n‑5)
 *            –524*a(n‑6) +2932*a(n‑7) –875*a(n‑8) –5880*a(n‑9) +4552*a(n‑10)
 *            +6378*a(n‑11) –7363*a(n‑12) –3518*a(n‑13) +6068*a(n‑14)
 *            +520*a(n‑15) –2576*a(n‑16) +352*a(n‑17) +448*a(n‑18)
 *            –128*a(n‑19); Initial values: a(1)=48, a(2)=222, a(3)=1049, a...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Coefficients of the recurrence (a(1) … a(19) are absolute values). */
static const int coeff[19] = {
    12,  -48,   26,   319,  -694,  -524,  2932,  -875,   -5880,
    4552, 6378, -7363, -3518, 6068,  520,  -2576,  352,   448,   -128
};

/* Cache of computed values.  cache[0] = a(1). */
static seq_t *cache = NULL;
static idx_t cap  = 0;   /* allocated size */
static idx_t size = 0;   /* number of stored values */

/* initialise the cache with the first 19 terms of the sequence */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    size = 19;
    cap  = 19;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) abort();      /* out of memory */

    /* initial terms of A235542 */
    cache[0]  = 48ull;
    cache[1]  = 222ull;
    cache[2]  = 1049ull;
    cache[3]  = 4348ull;
    cache[4]  = 17658ull;
    cache[5]  = 67012ull;
    cache[6]  = 247639ull;
    cache[7]  = 880814ull;
    cache[8]  = 3061429ull;
    cache[9]  = 10385988ull;
    cache[10] = 34604232ull;
    cache[11] = 113376954ull;
    cache[12] = 366386257ull;
    cache[13] = 1169544994ull;
    cache[14] = 3694214596ull;
    cache[15] = 11560700652ull;
    cache[16] = 35884753502ull;
    cache[17] = 110586677426ull;
    cache[18] = 338628074707ull;
}

/* grow the cache so that it can hold at least 'needed' entries */
static void grow_cache(idx_t needed)
{
    if (needed <= cap)
        return;
    size_t new_cap = cap ? cap * 2 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap   = (idx_t)new_cap;
}

/* recurrence implementation */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1, return 0 for out of range */
        return 0;

    init_cache();

    /* compute all missing terms up to n */
    if (n > size) {
        grow_cache(n);
        for (idx_t k = size + 1; k <= n; ++k) {
            /* compute a(k) using the 19‑term recurrence */
            seq_t possum = 0;   /* sum of positive coefficient terms   */
            seq_t negsum = 0;   /* sum of absolute values of negative terms */

            for (int j = 0; j < 19; ++j) {
                const int c = coeff[j];
                if (c == 0)
                    continue;
                idx_t idx = k - 2 - j;   /* a(k-(j+1)) -> cache index */
                seq_t term = (seq_t)c * cache[idx];
                if (c > 0)
                    possum += term;
                else
                    negsum += (seq_t)(-c) * cache[idx];
            }
            /* final value is possum - negsum (guaranteed non‑negative) */
            cache[k - 1] = possum - negsum;
        }
        size = n;
    }
    return cache[n - 1];
}
