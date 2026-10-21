/*
 * A231952  Number of (n+1)X(3+1) 0..1 arrays with every element equal to some
 *          horizontal, vertical, diagonal or antidiagonal neighbor, with top
 *          left element zero.
 * https://oeis.org/A231952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231952: Number of (n+1)x4 0..1 arrays with every element equal to some
   horizontal, vertical, diagonal or antidiagonal neighbor, with top left element zero.
   The sequence satisfies the linear recurrence
      a(n) = 14*a(n-1) + 6*a(n-2)  + 103*a(n-3) + 170*a(n-4)
             - 39*a(n-5)          - 13*a(n-6)  - 99*a(n-7)
             - 375*a(n-8)         - 29*a(n-9)  + 181*a(n-10)
             + 18*a(n-11)        - 24*a(n-12)
   for n ≥ 13, with initial values:
     a(1)=64, a(2)=1034, a(3)=14842, a(4)=220472, a(5)=3292876,
     a(6)=49124582, a(7)=732691918, a(8)=10928482748,
     a(9)=163005613290, a(10)=2431334781688,
     a(11)=36264925895498, a(12)=540914768944926.
   The function computes terms lazily and memoises results. */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values a(1) .. */
static size_t cache_cap = 0;     /* allocated capacity */
static int cache_size = 0;       /* number of stored terms */

/* ensure cache can hold at least n terms */
static void ensure_cache(size_t n)
{
    if (cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise the first twelve terms of the sequence */
static void init_first_terms(void)
{
    const seq_t init_vals[12] = {
        64ULL,
        1034ULL,
        14842ULL,
        220472ULL,
        3292876ULL,
        49124582ULL,
        732691918ULL,
        10928482748ULL,
        163005613290ULL,
        2431334781688ULL,
        36264925895498ULL,
        540914768944926ULL
    };
    ensure_cache(12);
    for (size_t i = 0; i < 12; ++i) cache[i] = init_vals[i];
    cache_size = 12;
}

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of defined range, returning 0 */
    if (!cache_size) init_first_terms();

    /* compute terms up to n if necessary */
    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* indices i-k correspond to a(i-k). Term indices start at 1. */
            seq_t val = 0;
            /* positive contributions first */
            val += (seq_t)14 * cache[i-2];   /* a(i-1) */
            val += (seq_t)6  * cache[i-3];   /* a(i-2) */
            val += (seq_t)103 * cache[i-4];  /* a(i-3) */
            val += (seq_t)170 * cache[i-5];  /* a(i-4) */
            /* negative contributions */
            val -= (seq_t)39  * cache[i-6];  /* a(i-5) */
            val -= (seq_t)13  * cache[i-7];  /* a(i-6) */
            val -= (seq_t)99  * cache[i-8];  /* a(i-7) */
            val -= (seq_t)375 * cache[i-9];  /* a(i-8) */
            val -= (seq_t)29  * cache[i-10]; /* a(i-9) */
            val += (seq_t)181 * cache[i-11]; /* a(i-10) */
            val += (seq_t)18  * cache[i-12]; /* a(i-11) */
            val -= (seq_t)24  * cache[i-13]; /* a(i-12) */
            ensure_cache((size_t)i);
            cache[i-1] = val;
        }
        cache_size = (int)n;
    }
    return cache[n-1];
}
