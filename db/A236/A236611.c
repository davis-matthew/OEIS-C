/*
 * A236611  Number of (n+1)X(1+1) 0..2 arrays colored with the maximum plus
 *          the upper median of every 2X2 subblock.
 * https://oeis.org/A236611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-3) +280*a(n-4) +376*a(n-5) +932*a(n-6)
 *            +1120*a(n-7) -1456*a(n-8) -1696*a(n-9) -768*a(n-10)
 *            -1152*a(n-11) for n > 13 Initial values: a(1)=81, a(2)=390,
 *            a(3)=1885, a(4)=8876, a(5)=41473, a(6)=193372, a(7)=912752,
 *            a(8)=4290712, a(9)=20084444, a(10)=93983432, a(11)=442021140,
 *            a(12)=2076...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[13] = {
    (seq_t)81ULL,
    (seq_t)390ULL,
    (seq_t)1885ULL,
    (seq_t)8876ULL,
    (seq_t)41473ULL,
    (seq_t)193372ULL,
    (seq_t)912752ULL,
    (seq_t)4290712ULL,
    (seq_t)20084444ULL,
    (seq_t)93983432ULL,
    (seq_t)442021140ULL,
    (seq_t)2076094832ULL,
    (seq_t)9730606384ULL
};

static seq_t *cache = NULL;          /* cached values, 0‑based (a(1) at cache[0]) */
static size_t cache_size = 0;        /* number of stored terms */

/* Resize cache if needed; after this cache_size == new_cap. */
static void ensure_cache(size_t new_cap) {
    if (new_cap <= cache_size) return;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);                /* out of memory */
    for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    if (cache) free(cache);
    cache = new_cache;
    cache_size = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for 0, defined as 0 */

    /* initial segment */
    if (n <= 13) return init_vals[n - 1];

    /* lazily allocate cache with initial values */
    if (!cache) {
        cache_size = 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = 0; i < 13; ++i) cache[i] = init_vals[i];
    }

    size_t idx = (size_t)n;                /* 1‑based index in terms of the function */

    if (idx > cache_size) {
        size_t old_cap = cache_size;        /* number of terms stored before expanding */
        ensure_cache(idx);                  /* now cache_size == idx */
        static const long long coeff[9] = {
            16LL, 280LL, 376LL, 932LL, 1120LL,
            -1456LL, -1696LL, -768LL, -1152LL
        };
        for (size_t i = old_cap; i < idx; ++i) {
            size_t cur = i + 1;             /* current n (1‑based) */
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 9; ++k) {
                size_t prev_index = cur - (k + 3);  /* 1‑based previous index */
                seq_t prev = cache[prev_index - 1];
                long long c = coeff[k];
                if (c >= 0) {
                    pos += (seq_t)c * prev;
                } else {
                    neg += (seq_t)(-c) * prev;
                }
            }
            cache[i] = pos - neg;            /* pos ≥ neg for valid terms */
        }
    }

    return cache[idx - 1];
}
