/*
 * A050512  a(n) = (a(n-1)*a(n-3) - a(n-2)^2) / a(n-4), with a(0) = 0, a(1) =
 *          a(2) = a(3) = 1, a(4) = -1.
 * https://oeis.org/A050512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = (a(n-1)*a(n-3) - a(n-2)^2) / a(n-4), with a(0)=0,
 *            a(1)=a(2)=a(3)=1, a(4)=-1
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 26, i64 n <= 37, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <type_traits>

/* --------------------------------------------------------------------
   Helper to evaluate the recurrence with the correct intermediate size.
   For integral types a 128‑bit intermediate is used; for other types (in
   particular arbitrary‑precision integer classes) the native operators
   are used.  The function template is never instantiated for the
   C compiler. ----------------------------------------------------- */
template<typename T>
typename std::enable_if<std::is_integral_v<T>, T>::type
recurrence(T a1, T a2, T a3, T a4)
{
    __int128 num = (__int128)a1 * a3 - (__int128)a2 * a2;
    return (T)(num / (__int128)a4);
}

template<typename T>
typename std::enable_if<!std::is_integral_v<T>, T>::type
recurrence(T a1, T a2, T a3, T a4)
{
    return (a1 * a3 - a2 * a2) / a4;
}

#else   /* C: all types are integral and __int128 is available */

static inline seq_t recurrence(seq_t a1, seq_t a2, seq_t a3, seq_t a4)
{
    __int128 num = (__int128)a1 * a3 - (__int128)a2 * a2;
    return (seq_t)(num / (__int128)a4);
}

#endif

static seq_t *cache      = NULL;   /* holds a(0)..a(k) */
static size_t cache_cap  = 0;       /* capacity of cache array */
static idx_t  cached_n   = (idx_t)-1;   /* highest index computed */

/* --------------------------------------------------------------------
   Compute a(n).  The cache is lazily initialised and extended on demand,
   allowing calls in arbitrary order.  No recursion is used.  All
   arithmetic is performed by the recurrence helper. --------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise cache array lazily */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
    }

    /* Grow cache to accommodate index n */
    while (cache_cap <= (size_t)n) {
        cache_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* First call: set base values */
    if (cached_n == (idx_t)-1) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)-1;
        cached_n = 4;
    }

    /* Return previously computed value */
    if (n <= cached_n)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t a1 = cache[(size_t)(i - 1)];   /* a(i-1) */
        seq_t a2 = cache[(size_t)(i - 2)];   /* a(i-2) */
        seq_t a3 = cache[(size_t)(i - 3)];   /* a(i-3) */
        seq_t a4 = cache[(size_t)(i - 4)];   /* a(i-4) */

        cache[(size_t)i] = recurrence(a1, a2, a3, a4);
    }

    cached_n = n;
    return cache[(size_t)n];
}
