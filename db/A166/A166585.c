/*
 * A166585  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*Σ_{j=1..11} a(n-j) – 120*a(n-12) for n > 12 a(12) =
 *            15*Σ_{j=1..11} a(12-j) – 120*a(0) + 1 a(0) = 1, a(1)=17,
 *            a(2)=272, a(3)=4352, a(4)=69632, a(5)=1114112, a(6)=17825792,
 *            a(7)=285212672, a(8)=4563402752, a(9)=73014444032,
 *            a(10)=1168231104512, a(11)=18691697672192
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[12] = {
    1ULL, 17ULL, 272ULL, 4352ULL, 69632ULL,
    1114112ULL, 17825792ULL, 285212672ULL,
    4563402752ULL, 73014444032ULL, 1168231104512ULL,
    18691697672192ULL
};

static seq_t *cache = NULL;     /* sequence values cached */
static idx_t   cache_len = 0;    /* number of elements currently stored */

/* Extend the cache up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                 /* cache already large enough */

    idx_t new_len = n + 1;        /* need space for 0 .. n */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)                    /* allocation failed: keep the old cache */
        return;
    cache = tmp;

    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i < 12) {            /* initial values */
            cache[i] = init[i];
        } else {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 11; ++j)
                sum += cache[i - j];
            if (i == 12) /* special case for a(12) */
                cache[i] = sum * 15 - cache[i - 12] * 120 + 1;
            else
                cache[i] = sum * 15 - cache[i - 12] * 120;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
