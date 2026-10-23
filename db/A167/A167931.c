/*
 * A167931  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=20, a(2)=380, a(3)=7220, a(4)=137180, a(5)=2606420,
 *            a(6)=49521980, a(7)=940917620, a(8)=17877434780,
 *            a(9)=339671260820, a(10)=6453753955580, a(11)=122621325156020,
 *            a(12)=2329805177964380, a(13)=44266298381323220,
 *            a(14)=841059669245141180, a(15)=15980133715657682420. For n≥16:
 *            a(n) =...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static const seq_t init_vals[16] = {
    (seq_t)1ULL,
    (seq_t)20ULL,
    (seq_t)380ULL,
    (seq_t)7220ULL,
    (seq_t)137180ULL,
    (seq_t)2606420ULL,
    (seq_t)49521980ULL,
    (seq_t)940917620ULL,
    (seq_t)17877434780ULL,
    (seq_t)339671260820ULL,
    (seq_t)6453753955580ULL,
    (seq_t)122621325156020ULL,
    (seq_t)2329805177964380ULL,
    (seq_t)44266298381323220ULL,
    (seq_t)841059669245141180ULL,
    (seq_t)15980133715657682420ULL
};

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* initialise cache with first 16 terms on first call */
    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 16; ++i) {
            cache[i] = init_vals[i];
        }
    }

    /* if the requested term is already computed, return it */
    if ((idx_t)cache_len > n) {
        return cache[(size_t)n];
    }

    /* extend cache up to index n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) {
        /* allocation failed – cannot compute further */
        return 0;
    }
    cache = tmp;

    /* compute missing terms */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i == 0) {           /* already cached */
            continue;
        }
        seq_t term = (seq_t)19 * cache[i - 1];
        if (i >= 16) {
            term -= (seq_t)189 * cache[i - 16];
        }
        if (i >= 17) {
            term += (seq_t)171 * cache[i - 17];
        }
        /* constants from the generating function: −1 for n=16,17 */
        if (i == 16 || i == 17) {
            term -= (seq_t)1;
        }
        cache[i] = term;
    }

    cache_len = new_len;
    return cache[(size_t)n];
}
