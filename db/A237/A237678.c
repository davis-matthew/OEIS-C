/*
 * A237678  Number of (n+1)X(1+1) 0..2 arrays colored with the upper median
 *          plus the lower median minus the minimum of every 2X2 subblock.
 * https://oeis.org/A237678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 72*a(n-3)+456*a(n-4)+1084*a(n-5)+3312*a(n-6)+712*a(n-7) *
 *            -9384*a(n-8)-26848*a(n-9)-73104*a(n-10)+19232*a(n-11) *
 *            +112256*a(n-12)+249216*a(n-13)+378368*a(n-14)-641536*a(n-15) *
 *            -627712*a(n-16)+231424*a(n-17)+372736*a(n-18) (n > 18) * Initial
 *            values (for n = 1 .. 18): * 81, 474, 2741, 15948...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Convert decimal string to seq_t. Works for built‑in unsigned types
   and for arbitrary‑precision types that support * and + with small
   integer literals. */
static seq_t str_to_seq(const char *s) {
    seq_t res = 0;
    while (*s) {
        res = res * (seq_t)10 + (seq_t)(*s - '0');
        ++s;
    }
    return res;
}

/* Cache of already computed terms.  No thread safety is required. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Initialise the cache with the 18 known initial values. */
static void init_cache(void) {
    if (cache_len != 0)
        return;

    cache_len = 19;                    /* indices 0 .. 18 */
    cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
    if (!cache) abort();

    cache[0] = 0;  /* unused, keeps indices 1..18 */
    static const char *init_str[19] = {
        "",                      /* 0 – unused */
        "81",                    /* 1 */
        "474",                   /* 2 */
        "2741",                  /* 3 */
        "15948",                 /* 4 */
        "93093",                 /* 5 */
        "541880",                /* 6 */
        "3152844",               /* 7 */
        "18356756",              /* 8 */
        "106893088",             /* 9 */
        "622285828",             /* 10 */
        "3622715432",            /* 11 */
        "21091482008",           /* 12 */
        "122794559800",          /* 13 */
        "714891615808",          /* 14 */
        "4162016007648",         /* 15 */
        "24230928689312",        /* 16 */
        "141070302809152",       /* 17 */
        "821296712783104"        /* 18 */
    };

    for (idx_t i = 1; i < cache_len; ++i)
        cache[i] = str_to_seq(init_str[i]);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 1 */
    init_cache();

    if (n < cache_len)        /* already known */
        return cache[n];

    /* Compute missing terms up to n (inclusive) */
    const int pos_coeff[] = {
        72, 456, 1084, 3312, 712,
        19232, 112256, 249216, 378368,
        231424, 372736
    };
    const int pos_off[]   = {3, 4, 5, 6, 7, 11, 12, 13, 14, 17, 18};

    const int neg_coeff[] = {9384, 26848, 73104, 641536, 627712};
    const int neg_off[]   = {8, 9, 10, 15, 16};

    size_t num_pos = sizeof(pos_coeff) / sizeof(pos_coeff[0]);
    size_t num_neg = sizeof(neg_coeff) / sizeof(neg_coeff[0]);

    idx_t old_len = cache_len;
    idx_t new_len = n + 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_len = new_len;

    for (idx_t i = old_len; i <= n; ++i) {
        seq_t val = 0;
        for (size_t j = 0; j < num_pos; ++j)
            val += (seq_t)pos_coeff[j] * cache[i - pos_off[j]];
        for (size_t j = 0; j < num_neg; ++j)
            val -= (seq_t)neg_coeff[j] * cache[i - neg_off[j]];
        cache[i] = val;
    }

    return cache[n];
}
