/*
 * A130532  a(n) + a(n - 1) is alternatively a square or a cube, a(1) = 1.
 * https://oeis.org/A130532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130532: a(1) = 1.
   For n > 1:
     if n is even, a(n) = smallest positive m such that a(n‑1)+m is a square;
     if n is odd,  a(n) = smallest positive m such that a(n‑1)+m is a cube.
   The implementation memoises the sequence so that calls can be made in any order. */

#include <stdlib.h>
#include <stddef.h> /* size_t, NULL */

#if defined(__cplusplus)
#include <vector>
#endif

/* Integer square root: floor(sqrt(x)) */
static seq_t isqrt(seq_t x)
{
    if (x == 0) return 0;
    seq_t low = 0;
    seq_t high = 1;
    /* grow high until high^2 > x */
    while (high <= x / high)            /* high*high <= x */
        high = high * 2;                 /* high *= 2 */
    low = high / 2;                      /* high>sqrt(x)>=high/2 */
    while (low < high) {
        seq_t mid = (low + high) / 2;
        if (mid <= x / mid)              /* mid*mid <= x */
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                      /* low-1 is floor sqrt */
}

/* Integer cube root: floor(cbrt(x)) */
static seq_t icbrt(seq_t x)
{
    if (x == 0) return 0;
    seq_t low = 0;
    seq_t high = 1;
    /* grow high until high^3 > x */
    while (1) {
        seq_t high2 = high * high;            /* high^2 */
        if (high2 <= x / high)                /* high^3 <= x */
            high = high * 2;                  /* high *= 2 */
        else
            break;
    }
    low = high / 2;                           /* high>cbrt(x)>=high/2 */
    while (low < high) {
        seq_t mid = (low + high) / 2;
        seq_t mid2 = mid * mid;
        if (mid2 <= x / mid)                  /* mid^3 <= x */
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                           /* floor cube root */
}

#if defined(__cplusplus)
static std::vector<seq_t> cache;
static size_t computed = 0;                      /* highest index computed */
static void ensure_capacity(idx_t n)
{
    if (cache.size() > (size_t)n) return;
    cache.resize((size_t)n + 1);
}
#else
static seq_t *cache = NULL;
static size_t capacity = 0;                      /* allocated size of cache */
static idx_t computed = 0;                       /* highest index computed */

static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= capacity) return;
    size_t newcap = capacity ? capacity : 1;
    while (newcap <= (size_t)n + 1)
        newcap <<= 1;                          /* grow by factor of 2 */
    seq_t *tmp = realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    capacity = newcap;
}
#endif

/* Compute a(n).  Supports arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined, but safe */

    /* initialise cache at first use */
    if (computed == 0) {
#if defined(__cplusplus)
        ensure_capacity(1);
        cache[1] = 1;
#else
        ensure_capacity(1);
        cache[1] = 1;
#endif
        computed = 1;
    }

    if (n > computed) {
#if defined(__cplusplus)
        ensure_capacity(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            if ((i & 1) == 0) {                /* even: next square */
                seq_t k = isqrt(prev) + 1;
                seq_t square = k * k;
                cache[i] = square - prev;
            } else {                           /* odd: next cube */
                seq_t k = icbrt(prev) + 1;
                seq_t cube = k * k * k;
                cache[i] = cube - prev;
            }
        }
        computed = n;
#else
        ensure_capacity(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            if ((i & 1) == 0) {                /* even: next square */
                seq_t k = isqrt(prev) + 1;
                seq_t square = k * k;
                cache[i] = square - prev;
            } else {                           /* odd: next cube */
                seq_t k = icbrt(prev) + 1;
                seq_t cube = k * k * k;
                cache[i] = cube - prev;
            }
        }
        computed = n;
#endif
    }
    return cache[n];
}
