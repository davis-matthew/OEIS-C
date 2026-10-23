/*
 * A186601  Number of (n+4)X5 binary arrays with every 5X5 subblock commuting
 *          with each horizontal and vertical neighbor 5X5 subblock.
 * https://oeis.org/A186601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-5) - a(n-6) - a(n-8) +
 *            a(n-9) + a(n-10) - a(n-13) for n>21, with initial values (offset
 *            1): a(1)=33554432, a(2)=6187, a(3)=11884, a(4)=42120,
 *            a(5)=156276, a(6)=403567, a(7)=707955, a(8)=912129, a(9)=992000,
 *            a(10)=1040400, a(11)=1082540, a(12)=1112608, a(13)=1...
 * Range:     u8 none, u16 none, u32 n <= 51, u64 n <= 157, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* base values a(1) … a(21); index 0 unused */
static const seq_t base_vals[22] = {
    0ULL,          /* 0 */
    33554432ULL,   /* 1 */
    6187ULL,       /* 2 */
    11884ULL,      /* 3 */
    42120ULL,      /* 4 */
    156276ULL,     /* 5 */
    403567ULL,     /* 6 */
    707955ULL,     /* 7 */
    912129ULL,     /* 8 */
    992000ULL,     /* 9 */
    1040400ULL,    /*10 */
    1082540ULL,    /*11 */
    1112608ULL,    /*12 */
    1140393ULL,    /*13 */
    1191603ULL,    /*14 */
    1348702ULL,    /*15 */
    1745361ULL,    /*16 */
    2463212ULL,    /*17 */
    3404473ULL,    /*18 */
    4453106ULL,    /*19 */
    5656033ULL,    /*20 */
    7140675ULL     /*21 */
};

static seq_t *cache = NULL;      /* cache[0] unused; 1‑based */
static idx_t cached_up_to = 0;   /* largest index cached so far */

/* Ensure cache[1..n] is computed, allocating or extending as needed */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n <= cached_up_to) return;

    /* Allocate new array of size n+1 (indices 0..n) */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) abort();

    /* Copy existing cached values into new array */
    for (idx_t i = 1; i <= cached_up_to; ++i) {
        new_cache[(size_t)i] = cache[(size_t)i];
    }

    /* Fill missing entries from cached_up_to+1 up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i <= 21) {
            new_cache[(size_t)i] = base_vals[(size_t)i];
        } else {
            /* recurrence */
            seq_t pos = new_cache[(size_t)(i-1)]
                       + new_cache[(size_t)(i-3)]
                       + new_cache[(size_t)(i-5)]
                       + new_cache[(size_t)(i-9)]
                       + new_cache[(size_t)(i-10)];
            seq_t neg = new_cache[(size_t)(i-4)]
                       + new_cache[(size_t)(i-6)]
                       + new_cache[(size_t)(i-8)]
                       + new_cache[(size_t)(i-13)];
            new_cache[(size_t)i] = pos - neg;
        }
    }

    /* Replace old cache */
    free(cache);
    cache = new_cache;
    cached_up_to = n;
}

/* External function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[(size_t)n];
}
