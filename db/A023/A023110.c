/*
 * A023110  Squares which remain squares when the last digit is removed.
 * https://oeis.org/A023110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-21) + 1443 * (a(n-7) – a(n-14)),  n > 22
   Initial values (1 ≤ n ≤ 23):
     a(1)  = 0
     a(2)  = 1
     a(3)  = 4
     a(4)  = 9
     a(5)  = 16
     a(6)  = 49
     a(7)  = 169
     a(8)  = 256
     a(9)  = 361
     a(10) = 1444
     a(11) = 3249
     a(12) = 18496
     a(13) = 64009
     a(14) = 237169
     a(15) = 364816
     a(16) = 519841
     a(17) = 2079364
     a(18) = 4678569
     a(19) = 26666896
     a(20) = 92294449
     a(21) = 341991049
     a(22) = 526060096
     a(23) = 749609641
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache      = NULL;          /* 0..cache_cap inclusive */
static size_t cache_cap  = 0;             /* last used index */

/* hard‑coded initial values – index 0 is unused */
static const seq_t initial[24] = {
    0ULL,   /* 0  (unused) */
    0ULL,   /* 1  */
    1ULL,   /* 2  */
    4ULL,   /* 3  */
    9ULL,   /* 4  */
    16ULL,  /* 5  */
    49ULL,  /* 6  */
    169ULL, /* 7  */
    256ULL, /* 8  */
    361ULL, /* 9  */
    1444ULL,/* 10 */
    3249ULL,/* 11 */
    18496ULL,/* 12 */
    64009ULL,/* 13 */
    237169ULL,/* 14 */
    364816ULL,/* 15 */
    519841ULL,/* 16 */
    2079364ULL,/* 17 */
    4678569ULL,/* 18 */
    26666896ULL,/* 19 */
    92294449ULL,/* 20 */
    341991049ULL,/* 21 */
    526060096ULL,/* 22 */
    749609641ULL /* 23 */
};

/* enlarge the cache so that indices up to n are available */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;

    size_t new_cap = n + 1;           /* indices 0 .. n inclusive   */
    seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_cache) return;          /* allocation failure – just fail */

    /* copy any previously computed entries */
    if (cache) {
        for (size_t i = 1; i <= cache_cap; ++i)
            new_cache[i] = cache[i];
    }

    /* copy the hard‑coded initial block */
    idx_t limit = (idx_t)new_cap - 1;
    if (limit > 23) limit = 23;
    for (idx_t i = 1; i <= limit; ++i)
        new_cache[i] = initial[i];

    /* compute further values by recurrence */
    idx_t start = cache_cap + 1;
    if (start <= 23) start = 24; /* recurrence only for n > 22 */

    for (idx_t i = start; i < new_cap; ++i) {
        seq_t diff = new_cache[i-7] - new_cache[i-14];
        seq_t prod = diff * (seq_t)1443;
        new_cache[i] = new_cache[i-21] + prod;
    }

    free(cache);
    cache      = new_cache;
    cache_cap  = new_cap - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – guard for out of range */
    ensure_cache(n);
    return cache[n];
}
