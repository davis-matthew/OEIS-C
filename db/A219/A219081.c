/*
 * A219081  Number of 4 X n binary arrays with every 0 a horizontal, diagonal
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A219081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the 13‑term linear recurrence a(n) = 10 a(n-1) + 44
 *            a(n-2) + 384 a(n-3) + 1502 a(n-4) + 1612 a(n-5) -478 a(n-6)
 *            -4742 a(n-7) -13053 a(n-8) -13178 a(n-9) -7218 a(n-10) -8982
 *            a(n-11) -108 a(n-12) + 3888 a(n-13). Initial values (1‑based):
 *            a(1)=1, a(2)=149, a(3)=2469, a(4)=32945, a(5)=49...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms (1‑based index → 0‑based array). */
static seq_t *cache   = NULL;
static idx_t  cap     = 0;
static idx_t  computed = 0;

static const seq_t init_seq[13] = {
    1ULL,
    149ULL,
    2469ULL,
    32945ULL,
    494713ULL,
    7561349ULL,
    113959413ULL,
    1715622809ULL,
    25868247057ULL,
    390055483301ULL,
    5880485889397ULL,
    88654851159857ULL,
    1336593648057193ULL
};

/* Ensure that the cache can hold at least `needed` terms (1‑based). */
static void ensure_capacity(idx_t needed) {
    if (cap >= needed)
        return;
    idx_t newcap = cap ? cap << 1 : 16;
    while (newcap < needed)
        newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cap = newcap;
}

/* Sequence function */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* undefined; safety return */

    /* Initialise cache on first call. */
    if (!cache) {
        ensure_capacity(13);
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init_seq[i];
        computed = 13;
    }

    /* Return already computed value. */
    if (n <= computed)
        return cache[n - 1];

    /* Grow cache to hold up to n terms. */
    ensure_capacity(n);

    /* Compute terms from computed + 1 up to n. */
    for (idx_t idx = computed; idx < n; ++idx) {
        /* idx is 0‑based for cache, corresponds to a(idx+1). */
        seq_t pos = 0;
        seq_t neg = 0;

        pos += cache[idx - 1] * (seq_t)10;
        pos += cache[idx - 2] * (seq_t)44;
        pos += cache[idx - 3] * (seq_t)384;
        pos += cache[idx - 4] * (seq_t)1502;
        pos += cache[idx - 5] * (seq_t)1612;
        pos += cache[idx - 13] * (seq_t)3888;   /* +3888*a(n-13) */

        neg += cache[idx - 6] * (seq_t)478;
        neg += cache[idx - 7] * (seq_t)4742;
        neg += cache[idx - 8] * (seq_t)13053;
        neg += cache[idx - 9] * (seq_t)13178;
        neg += cache[idx - 10] * (seq_t)7218;
        neg += cache[idx - 11] * (seq_t)8982;
        neg += cache[idx - 12] * (seq_t)108;

        cache[idx] = pos - neg;
    }

    computed = n;
    return cache[n - 1];
}
