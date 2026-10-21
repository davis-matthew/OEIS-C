/*
 * A229537  Number of defective 3-colorings of a 4 X n 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly one
 *          mistake, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence (order 12). a(1) = 0, a(2) = 84,
 *            a(3) = 356, a(4) = 3064, a(5) = 19276, a(6) = 130854, a(7) =
 *            833108, a(8) = 5305746, a(9) = 33122792, a(10) = 205196216,
 *            a(11) = 1258781344, a(12) = 7670081250, a(13) = 46440194380,
 *            a(14) = 279724889174. For n > 14: a(n) = 6*a(n-1) + 23...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised values, 1‑based indexing */
static idx_t computed_up_to = 0;   /* largest n already computed */
static const int coeff[12] = {
    6, 23, -102, -288, 250, 787, -238, -741,
    124, 196, -16, -16
};

static void ensure_capacity(idx_t n) {
    /* make sure cache has indices 0..n available */
    idx_t needed = n + 1;          /* include index 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
    } else if (needed > computed_up_to + 1) {
        idx_t new_cap = (computed_up_to + 1) * 2;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset is 1, but clipping for safety */
    /* initial 14 terms (0‑based in array, 1‑based externally) */
    static const seq_t init[14] = {
        0ULL,          /* a(1) */
        84ULL,         /* a(2) */
        356ULL,        /* a(3) */
        3064ULL,       /* a(4) */
        19276ULL,      /* a(5) */
        130854ULL,     /* a(6) */
        833108ULL,     /* a(7) */
        5305746ULL,    /* a(8) */
        33122792ULL,   /* a(9) */
        205196216ULL,  /* a(10) */
        1258781344ULL, /* a(11) */
        7670081250ULL, /* a(12) */
        46440194380ULL,/* a(13) */
        279724889174ULL/* a(14) */
    };

    /* initialise cache up to 14 if not done already */
    if (computed_up_to < 14) {
        if (cache == NULL) {
            cache = (seq_t *)malloc(15 * sizeof(seq_t)); /* indices 0..14 */
        } else if ((idx_t)malloc(0) == NULL) {
            /* nothing to do, assume capacity is already >=15 */
        }
        for (idx_t i = 1; i <= 14; ++i) {
            cache[i] = init[i - 1];
        }
        computed_up_to = 14;
    }

    if (n <= computed_up_to) return cache[n];

    ensure_capacity(n);

    /* compute values sequentially up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t acc = 0;
        for (int k = 0; k < 12; ++k) {
            idx_t j = i - 1 - k; /* index of a(i-k) */
            seq_t term = cache[j];
            if (coeff[k] >= 0) {
                acc += term * (seq_t)coeff[k];
            } else {
                acc -= term * (seq_t)(-coeff[k]);
            }
        }
        cache[i] = acc;
    }
    computed_up_to = n;
    return cache[n];
}
