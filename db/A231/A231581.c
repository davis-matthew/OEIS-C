/*
 * A231581  Number of n X 2 0..3 arrays with no element less than a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors.
 * https://oeis.org/A231581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   16‑term linear recurrence * a(n) = 4*a(n-1) +6*a(n-2) +38*a(n-3)
 *            -161*a(n-4) -292*a(n-5) * -953*a(n-6) +1185*a(n-7) +4928*a(n-8)
 *            +12336*a(n-9) * +7076*a(n-10) -7076*a(n-11) -24464*a(n-12)
 *            -24608*a(n-13) * -18624*a(n-14) -6720*a(n-15) -2304*a(n-16) *
 *            With initial values * a(1)=4, a(2)=28, a(3)=124...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based cache of a(n) values */
static size_t cache_len = 0;    /* highest n stored in cache (0 if empty) */
static size_t cache_cap = 0;    /* capacity of cache array (excluding index 0) */

/* Grow cache to accommodate at least 'need' + 1 elements (index 0 unused). */
static void grow_cache(size_t need)
{
    if (need + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!newp)
        exit(EXIT_FAILURE);
    cache = newp;
    cache_cap = new_cap;
}

/* Initialize cache with the first 16 terms. */
static void init_cache(void)
{
    const seq_t init_vals[17] = {
        0ULL,                /* dummy 0th entry (unused) */
        4ULL,                  /* a(1) */
        28ULL,                 /* a(2) */
        124ULL,                /* a(3) */
        602ULL,                /* a(4) */
        2776ULL,               /* a(5) */
        12922ULL,              /* a(6) */
        60720ULL,              /* a(7) */
        286047ULL,             /* a(8) */
        1335296ULL,            /* a(9) */
        6256326ULL,            /* a(10) */
        29377828ULL,           /* a(11) */
        137595239ULL,          /* a(12) */
        644951590ULL,          /* a(13) */
        3024402309ULL,         /* a(14) */
        14175895645ULL,        /* a(15) */
        66459189937ULL         /* a(16) */
    };
    grow_cache(16);
    for (size_t i = 1; i <= 16; ++i)
        cache[i] = init_vals[i];
    cache_len = 16;
}

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1; return 0 */

    if (cache_len == 0)
        init_cache();

    if ((size_t)n <= cache_len)
        return cache[n];

    /* Coefficients of the recurrence (index 1..16). */
    static const int coeff[17] = {
        0,    /* unused */
        4,    /* a(n-1) */
        6,    /* a(n-2) */
        38,   /* a(n-3) */
        -161, /* a(n-4) */
        -292, /* a(n-5) */
        -953, /* a(n-6) */
        1185, /* a(n-7) */
        4928, /* a(n-8) */
        12336,/* a(n-9) */
        7076, /* a(n-10) */
        -7076,/* a(n-11) */
        -24464,/* a(n-12) */
        -24608,/* a(n-13) */
        -18624,/* a(n-14) */
        -6720,/* a(n-15) */
        -2304 /* a(n-16) */
    };

    grow_cache((size_t)n);
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
        seq_t term = 0;
        for (int i = 1; i <= 16; ++i) {
            seq_t prev = cache[k - i];
            int c = coeff[i];
            if (c >= 0)
                term += (seq_t)c * prev;
            else
                term -= (seq_t)(-c) * prev;
        }
        cache[k] = term;
    }
    cache_len = (size_t)n;
    return cache[n];
}
