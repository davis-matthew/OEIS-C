/*
 * A221966  Number of -7..7 arrays of length n with the sum ahead of each
 *          element differing from the sum following that element by 7 or
 *          less.
 * https://oeis.org/A221966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A221966:
   a(n) = 7*a(n-1) + 32*a(n-2) + 112*a(n-5) + 256*a(n-6)
          - (56*a(n-3) + 160*a(n-4) + 64*a(n-7) + 128*a(n-8))
   with a(1)=15, a(2)=225, a(3)=1695, a(4)=17025,
   a(5)=159615, a(6)=1535745, a(7)=14661375, a(8)=140355585. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* sequence cache: cache[0] holds a(1) */
static seq_t *cache = NULL;
static idx_t cached_size = 0;

/* initialise cache with first eight terms */
static void init_cache(void)
{
    if (cached_size > 0)
        return;  /* already initialised */

    cache = (seq_t*)malloc(8 * sizeof(seq_t));
    if (!cache) abort();
    cache[0] = (seq_t)15;           /* a(1) */
    cache[1] = (seq_t)225;          /* a(2) */
    cache[2] = (seq_t)1695;         /* a(3) */
    cache[3] = (seq_t)17025;        /* a(4) */
    cache[4] = (seq_t)159615;       /* a(5) */
    cache[5] = (seq_t)1535745;      /* a(6) */
    cache[6] = (seq_t)14661375;     /* a(7) */
    cache[7] = (seq_t)140355585;    /* a(8) */
    cached_size = 8;
}

/* ensure cache contains at least n terms (n >= 1) */
static void ensure_cache(idx_t n)
{
    if (cached_size == 0)
        init_cache();

    if (n <= cached_size)
        return;

    /* grow buffer */
    seq_t *new_cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* compute missing terms via the recurrence */
    for (idx_t i = cached_size + 1; i <= n; ++i) {
        idx_t idx = i - 1;               /* 0‑based index of a(i) */
        seq_t a1 = cache[idx - 1];
        seq_t a2 = cache[idx - 2];
        seq_t a5 = cache[idx - 5];
        seq_t a6 = cache[idx - 6];
        seq_t a3 = cache[idx - 3];
        seq_t a4 = cache[idx - 4];
        seq_t a7 = cache[idx - 7];
        seq_t a8 = cache[idx - 8];

        /* compute the recurrence */
        seq_t sum1 = 7 * a1 + 32 * a2 + 112 * a5 + 256 * a6;
        seq_t sum2 = 56 * a3 + 160 * a4 + 64 * a7 + 128 * a8;
        cache[idx] = sum1 - sum2; /* unsigned subtraction safe for positive results */
    }

    cached_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined, return 0 for safety */
    ensure_cache(n);
    return cache[n - 1];
}
