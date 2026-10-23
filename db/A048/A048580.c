/*
 * A048580  Pisot sequence L(3,10).
 * https://oeis.org/A048580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( a(n-1)^2 / a(n-2) ), with a(0)=3, a(1)=10
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    #define USE_INT128_C 1
#elif defined(__cplusplus)
    #include <type_traits>
    #define USE_INT128_CPP 1
#endif

static seq_t ceil_div_sq(seq_t a, seq_t b)
{
#if defined(USE_INT128_C)
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)a;
    seq_t q = (seq_t)(prod / b);
    seq_t r = (seq_t)(prod % b);
    if (r != 0) ++q;
    return q;
#elif defined(USE_INT128_CPP)
    if constexpr (std::is_integral<seq_t>::value && sizeof(seq_t) <= 8) {
        unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)a;
        seq_t q = (seq_t)(prod / b);
        seq_t r = (seq_t)(prod % b);
        if (r != 0) ++q;
        return q;
    } else {
        /* fallback for arbitrary‑precision seq_t */
        seq_t num = a * a;
        seq_t q = num / b;
        seq_t r = num % b;
        if (r != 0) ++q;
        return q;
    }
#else
    /* no 128‑bit support – use native multiplication (works when no overflow) */
    seq_t num = a * a;
    seq_t q   = num / b;
    seq_t r   = num % b;
    if (r != 0) ++q;
    return q;
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t last_computed = (idx_t)-1; /* -1 means nothing computed yet */
    const seq_t first = 3;
    const seq_t second = 10;

    if (n == 0) return first;
    if (n == 1) return second;

    /* ensure the cache holds at least the first two terms */
    if (!cache) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return zero as a safe fallback */
        cache[0] = first;
        cache[1] = second;
        last_computed = 1;
    }

    /* extend the cache if necessary */
    if (last_computed < n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – return zero */
        cache = tmp;
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = ceil_div_sq(cache[i - 1], cache[i - 2]);
        }
        last_computed = n;
    }

    return cache[n];
}
