/*
 * A218228  Hilltop maps: number of nX3 binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal, vertical
 *          or antidiagonal neighbor in a random 0..2 nX3 array.
 * https://oeis.org/A218228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 7*a(n-2) + 7*a(n-3) + 5*a(n-4) + 9*a(n-5) +
 *            9*a(n-6) + 9*a(n-7) + 8*a(n-8) + 4*a(n-9) + a(n-10) + a(n-11) +
 *            a(n-12) + a(n-13) - a(n-14) - a(n-15) - a(n-16) - a(n-17)
 *            Initial values: a(1)=7, a(2)=61, a(3)=497, a(4)=3969,
 *            a(5)=31739, a(6)=253825, a(7)=2029849, a(8)=16232833, a(9)=...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#define SEED_COUNT 19
static const seq_t SEEDS[SEED_COUNT] = {
    (seq_t)7,                /* a(1) */
    (seq_t)61,               /* a(2) */
    (seq_t)497,              /* a(3) */
    (seq_t)3969,             /* a(4) */
    (seq_t)31739,            /* a(5) */
    (seq_t)253825,           /* a(6) */
    (seq_t)2029849,          /* a(7) */
    (seq_t)16232833,         /* a(8) */
    (seq_t)129815047,        /* a(9) */
    (seq_t)1038139593,       /* a(10) */
    (seq_t)8302071585,       /* a(11) */
    (seq_t)66392220265,      /* a(12) */
    (seq_t)530943014251,     /* a(13) */
    (seq_t)4245986702321,    /* a(14) */
    (seq_t)33955438893389,   /* a(15) */
    (seq_t)271543909879097,  /* a(16) */
    (seq_t)2171554761048415, /* a(17) */
    (seq_t)17366068280933465, /* a(18) */
    (seq_t)138877606472351657 /* a(19) */
};

static seq_t *cache = NULL;     /* array storing computed values */
static size_t cached_len = 0;    /* how many entries currently stored */

/* Ensure cache holds at least n values (n ≥ 1). */
static void ensure_cache_size(idx_t n)
{
    if (cache != NULL && cached_len >= (size_t)n)
        return;          /* already large enough */

    /* Allocate a new buffer big enough for future growth. */
    size_t new_len = ((size_t)n * 2) >= 64 ? (size_t)n * 2 : 64;
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
    if (!new_cache)
        return;          /* allocation failure: caller will handle */

    /* Copy existing values. */
    for (size_t i = 0; i < cached_len; ++i)
        new_cache[i] = cache[i];

    /* If we have no old cache, initialise with the seed values. */
    if (cached_len == 0) {
        for (size_t i = 0; i < SEED_COUNT && i < new_len; ++i)
            new_cache[i] = SEEDS[i];
        cached_len = SEED_COUNT;
    }

    free(cache);
    cache = new_cache;
}

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined, return 0 */

    ensure_cache_size(n);

    if ((size_t)n <= cached_len)
        return cache[(size_t)n - 1];

    /* Compute values from cached_len+1 up to n */
    for (size_t i = cached_len + 1; i <= (size_t)n; ++i) {
        size_t idx = i - 1;  /* zero‑based index in cache */

        /* For indices ≤ SEED_COUNT we already have seeds */
        if (idx < SEED_COUNT) {
            cache[idx] = SEEDS[idx];
            continue;
        }

        /* Recurrence requires the previous 17 values */
        seq_t val = (seq_t)7 * cache[idx - 1];
        val += (seq_t)7 * cache[idx - 2];
        val += (seq_t)7 * cache[idx - 3];
        val += (seq_t)5 * cache[idx - 4];
        val += (seq_t)9 * cache[idx - 5];
        val += (seq_t)9 * cache[idx - 6];
        val += (seq_t)9 * cache[idx - 7];
        val += (seq_t)8 * cache[idx - 8];
        val += (seq_t)4 * cache[idx - 9];
        val += cache[idx - 10];
        val += cache[idx - 11];
        val += cache[idx - 12];
        val += cache[idx - 13];
        val -= cache[idx - 14];
        val -= cache[idx - 15];
        val -= cache[idx - 16];
        val -= cache[idx - 17];

        cache[idx] = val;
    }
    cached_len = (size_t)n;
    return cache[(size_t)n - 1];
}
