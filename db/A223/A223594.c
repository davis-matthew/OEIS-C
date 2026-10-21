/*
 * A223594  Petersen graph (8,2) coloring a rectangular array: number of n X 3
 *          0..15 arrays where 0..15 label nodes of a graph with edges 0,1 0,8
 *          8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11 11,13 4,5 4,12
 *          12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every array movement to
 *          a horizontal, diagonal or ant...
 * https://oeis.org/A223594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A223594: number of n×3 arrays with entries 0..15
 * satisfying the Petersen graph 8‑2 coloring constraints.
 *
 * Recurrence (empirical, verified for many terms):
 *   a(n) = 23 a(n-1) – 153 a(n-2) + 217 a(n-3) + 258 a(n-4)
 *          – 456 a(n-5) – 104 a(n-6) + 192 a(n-7)
 *
 * Initial values (offset 1):
 *   a(1)=144, a(2)=1504, a(3)=16192, a(4)=176224, a(5)=1931968,
 *   a(6)=21308000, a(7)=236213312
 *
 * The function computes a(n) for any n ≥ 1, memoizing intermediate
 * values.  It is efficient for n up to at least 10 000.
 */

#include <stdlib.h>

/* Recurrence coefficients (index 0 → a(n-1), index 6 → a(n-7)) */
static const int coeffs[7] = {23, -153, 217, 258, -456, -104, 192};

/* Memoization cache */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Helper: allocate an array of seq_t of given length (C/C++ compatible) */
static seq_t *alloc_seq(size_t len)
{
    /* In C++ the cast is required; in C it is implicit. */
    return (seq_t *)malloc(len * sizeof(seq_t));
}

/* Helper: reallocate an array of seq_t to a new length (C/C++ compatible) */
static seq_t *grow_seq(seq_t *ptr, size_t new_len)
{
    return (seq_t *)realloc(ptr, new_len * sizeof(seq_t));
}

/* Main function */
seq_t a(idx_t n)
{
    /* a(0) is unused because the sequence offset is 1 */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache with the first seven terms on first call */
    if (cache == NULL) {
        cache_size = 7;
        cache = alloc_seq(cache_size);
        if (cache == NULL)
            return (seq_t)0; /* allocation failed */

        cache[0] = (seq_t)144;       /* a(1) */
        cache[1] = (seq_t)1504;      /* a(2) */
        cache[2] = (seq_t)16192;     /* a(3) */
        cache[3] = (seq_t)176224;    /* a(4) */
        cache[4] = (seq_t)1931968;   /* a(5) */
        cache[5] = (seq_t)21308000;  /* a(6) */
        cache[6] = (seq_t)236213312; /* a(7) */
    }

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Extend cache up to n */
    size_t new_size = (size_t)n;
    seq_t *new_cache = grow_seq(cache, new_size);
    if (new_cache == NULL)
        return (seq_t)0; /* allocation failed */
    cache = new_cache;

    /* Compute new terms using the recurrence */
    for (size_t i = cache_size; i < new_size; ++i) {
        seq_t sum = (seq_t)0;
        for (int j = 0; j < 7; ++j) {
            seq_t prev = cache[i - 1 - j];
            if (coeffs[j] >= 0)
                sum += prev * (seq_t)coeffs[j];
            else
                sum -= prev * (seq_t)(-coeffs[j]); /* subtract magnitude */
        }
        cache[i] = sum;
    }

    cache_size = new_size;
    return cache[(size_t)n - 1];
}
