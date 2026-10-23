/*
 * A122441  Expansion of 2*(sqrt(1+8x)-3)/(sqrt(1+8x)-5).
 * https://oeis.org/A122441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 24, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A122441 – coefficients of 2*(sqrt(1+8x)-3)/(sqrt(1+8x)-5)
    Recurrence (valid for n ≥ 1):
        (4+8*n)a(n) + (-10-23*n)a(n+1) + (-3*n-6)a(n+2) = 0
        a(0)=1, a(1)=-1, a(2)=1
    The implementation memoises previously computed terms and uses
    128‑bit arithmetic when seq_t is an integral type to avoid
    intermediate overflow. When seq_t is an arbitrary‑precision class,
    the native big‑integer operators are used.
*/
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/* incremental cache for the sequence */
static seq_t *cache = NULL;
static idx_t  capacity = 0;
/* greatest index that has been computed; -1 means uncached */
static idx_t  cached_n = (idx_t)-1;

/* initialise the first three terms once */
static void init_cache(void)
{
    if (cached_n != (idx_t)-1)
        return;
    capacity = 3;
    cache = (seq_t*)malloc(capacity * sizeof(seq_t));
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)-1;
    cache[2] = (seq_t)1;
    cached_n = 2;
}

/* grow the cache to at least 'need+1' entries */
static void grow_cache(idx_t need)
{
    if (need < capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 1;
    if (new_cap < need + 1)
        new_cap = need + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    capacity = new_cap;
}

/* compute a(k+2) from a(k) and a(k+1) */
#ifdef __cplusplus
static inline seq_t compute_next(idx_t k, seq_t a_k, seq_t a_k1)
{
    if constexpr (std::is_integral<seq_t>::value) {
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
        using int128 = __int128_t;
#else
        using int128 = long long;
#endif
        int128 term1 = (int128)(4 + 8*k) * (int128)a_k;
        int128 term2 = (int128)(10 + 23*k) * (int128)a_k1;
        int128 num   = term1 - term2;
        int128 den   = (int128)(3*k + 6);
        return (seq_t)(num / den);
    } else {
        seq_t term1 = a_k * (seq_t)(4 + 8*k);
        seq_t term2 = a_k1 * (seq_t)(10 + 23*k);
        seq_t num   = term1 - term2;
        seq_t den   = (seq_t)(3*k + 6);
        return num / den;
    }
}
#else
static inline seq_t compute_next(idx_t k, seq_t a_k, seq_t a_k1)
{
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
    __int128_t term1 = (__int128_t)(4 + 8*k) * (__int128_t)a_k;
    __int128_t term2 = (__int128_t)(10 + 23*k) * (__int128_t)a_k1;
    __int128_t num   = term1 - term2;
    __int128_t den   = (__int128_t)(3*k + 6);
    return (seq_t)(num / den);
#else
    /* Fallback – may overflow if seq_t is 64‑bit and k large */
    return (seq_t)(( (seq_t)(4 + 8*k) * a_k - (seq_t)(10 + 23*k) * a_k1 ) / (3*k + 6));
#endif
}
#endif

seq_t a(idx_t n)
{
    if (n == (idx_t)-1) /* UB for unsigned type, just return 0 */
        return (seq_t)0;
    init_cache();
    if (n <= cached_n)
        return cache[n];
    grow_cache(n);
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* compute a(i) from a(i-2) and a(i-1) */
        cache[i] = compute_next(i - 2, cache[i - 2], cache[i - 1]);
    }
    cached_n = n;
    return cache[n];
}
