/*
 * A219399  Unchanging value maps: number of nX4 binary arrays indicating the
 *          locations of corresponding elements unequal to no king-move
 *          neighbor in a random 0..1 nX4 array.
 * https://oeis.org/A219399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) -7*a(n-2) +8*a(n-3) +19*a(n-4) -19*a(n-5)
   -17*a(n-6) +26*a(n-7) -13*a(n-8) -41*a(n-9) -11*a(n-10) -14*a(n-11)
   +18*a(n-12) +45*a(n-13) -11*a(n-14) -36*a(n-15) +8*a(n-16) +17*a(n-17)
   -5*a(n-18) -5*a(n-19) +2*a(n-20)   for n > 21.
   Initial values: a(1)=7, a(2)=7, a(3)=43, a(4)=165, a(5)=649,
   a(6)=2527, a(7)=10011, a(8)=39787, a(9)=157725, a(10)=623929,
   a(11)=2468377, a(12)=9771861, a(13)=38692747, a(14)=153187901,
   a(15)=606420643, a(16)=2400630287, a(17)=9503657933,
   a(18)=37623622731, a(19)=148945583497, a(20)=589647430651,
   a(21)=2334303207355. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* coefficients for a(n-1) … a(n-20) */
static const int coeff[20] = {
    5,  -7,   8,  19,  -19,  -17,  26,  -13,  -41,  -11,
   -14,  18,  45,  -11,  -36,   8,  17,   -5,   -5,   2
};

/* first 21 terms of the sequence (indices 0..20 -> a(1)..a(21)) */
static const seq_t init[21] = {
    7ULL,
    7ULL,
    43ULL,
    165ULL,
    649ULL,
    2527ULL,
    10011ULL,
    39787ULL,
    157725ULL,
    623929ULL,
    2468377ULL,
    9771861ULL,
    38692747ULL,
    153187901ULL,
    606420643ULL,
    2400630287ULL,
    9503657933ULL,
    37623622731ULL,
    148945583497ULL,
    589647430651ULL,
    2334303207355ULL
};

/* multiply seq_t by small unsigned integer, avoiding overflow when possible */
static inline seq_t mul_uint(seq_t a, unsigned int b)
{
#ifndef __cplusplus
    /* use unsigned __int128 if available, otherwise rely on native overflow */
    unsigned __int128 prod = (unsigned __int128)a * b;
    return (seq_t)prod;
#else
    /* In C++ the compiler can promote the expression appropriately. */
    return (seq_t)((seq_t)a * (seq_t)b);
#endif
}

#ifdef __cplusplus
/* --------------------- C++ implementation --------------------- */
#include <vector>

static std::vector<seq_t> cache_vec;   /* index 0 unused, match OEIS offset 1 */
static bool cache_ready = false;

static void compute_to(idx_t n)
{
    if (!cache_ready) {
        cache_vec.resize(1);   /* element 0 unused */
        cache_ready = true;
    }
    size_t cur = cache_vec.size() - 1;
    if ((idx_t)cur >= n) return;

    cache_vec.resize(n + 1);   /* ensure space up to n */

    for (idx_t i = (idx_t)cur + 1; i <= n; ++i) {
        if (i <= 21) {
            cache_vec[i] = init[i - 1];
        } else {
            seq_t sum = 0;
            for (int j = 1; j <= 20; ++j) {
                int c = coeff[j - 1];
                seq_t prod = mul_uint(cache_vec[i - j],
                                      (unsigned int)(c > 0 ? c : -c));
                sum += (c > 0) ? prod : -prod;
            }
            cache_vec[i] = sum;
        }
    }
}

#else
/* --------------------- C implementation --------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* highest index stored (+1) */
static size_t cache_cap = 0;   /* allocated size */

static void compute_to(idx_t n)
{
    if ((idx_t)cache_len >= n) return;

    /* allocate if first time */
    if (!cache) {
        cache_cap = 2; /* enough for index 0 and 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* grow if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) abort();
        cache = ptr;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 21) {
            cache[i] = init[i - 1];
        } else {
            seq_t sum = 0;
            for (int j = 1; j <= 20; ++j) {
                int c = coeff[j - 1];
                seq_t prod = mul_uint(cache[i - j],
                                      (unsigned int)(c > 0 ? c : -c));
                sum += (c > 0) ? prod : -prod;
            }
            cache[i] = sum;
        }
    }
    cache_len = n;
}
#endif

/* --------------------- API function --------------------- */
seq_t a(idx_t n)
{
    compute_to(n);
#ifdef __cplusplus
    return cache_vec[n];
#else
    return cache[n];
#endif
}
