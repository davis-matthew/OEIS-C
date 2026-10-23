/*
 * A162592  Hypotenuse numbers A009003 which cannot be represented as sum of 2
 *          distinct nonzero squares.
 * https://oeis.org/A162592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that has at least one prime
 *            factor ≡ 1 (mod 4) and at least one prime factor ≡ 3 (mod 4) to
 *            an odd exponent. Such numbers are hypotenuse numbers that are
 *            not themselves expressible as a sum of two squares
 * Range:     u8 n <= 50, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Helper that decides whether a 64‑bit integer satisfies the    */
/*  required prime‑factor pattern.  It is intentionally simple  */
/*  (trial division) because the sequence grows slowly and       */
/*  we only need to handle values that comfortably fit in 64‑bit. */
/* ------------------------------------------------------------ */
static bool good_candidate(uint64_t n)
{
    if (n < 15)              /* undefined candidates are ignored   */
        return false;

    bool has1 = false;       /* at least one prime ≡1 mod4          */
    bool has3odd = false;    /* at least one prime ≡3 mod4 odd exp  */

    uint64_t t = n;

    /* strip out factor 2 – it never influences the conditions   */
    while ((t & 1) == 0)
        t >>= 1;

    uint64_t i = 3;
    while (i * i <= t) {
        if (t % i == 0) {
            uint64_t cnt = 0;
            while (t % i == 0) {
                t /= i;
                ++cnt;
            }
            if ((i & 3ULL) == 1ULL)         has1 = true;
            if (((i & 3ULL) == 3ULL) && (cnt & 1ULL))
                has3odd = true;
        }
        i += 2;           /* only odd divisors are needed now   */
    }

    if (t > 1) {           /* remaining factor is prime           */
        if ((t & 3ULL) == 1ULL)       has1 = true;
        if ((t & 3ULL) == 3ULL)       has3odd = true;
    }

    return has1 && has3odd;
}

/* ------------------------------------------------------------ */
/*  Cache of already computed values.  The cache grows as
    needed and remembers the last tested integer so that calls
    in arbitrary order are fast.                                */
/* ------------------------------------------------------------ */
static seq_t  *cache = NULL;      /* 1‑based array of terms                       */
static size_t cache_cap = 0;      /* allocated capacity of cache                   */
static idx_t  last_term  = 0;     /* number of terms that have been computed      */
static uint64_t last_test = 0;    /* last integer that has been examined          */

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);
    cache = new_arr;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined, but keep behaviour sane */
        return (seq_t)0;

    ensure_cache_cap((size_t)n);

    while ((size_t)last_term < n) {
        ++last_test;                    /* next integer to check */
        if (good_candidate((uint64_t)last_test)) {
            ++last_term;
            cache[last_term - 1] = (seq_t)last_test;
        }
    }
    return cache[n - 1];
}
