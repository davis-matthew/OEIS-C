/*
 * A079719  a(n) = n + floor(Sum_{k<n} a(k)/2) with a(0)=0.
 * https://oeis.org/A079719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(Sum_{k<n} a(k)/2), a(0)=0
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 53, u64 n <= 107,
 *            u128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include <stdlib.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* For integral seq_t of up to 64 bits use a 128‑bit accumulator. */
    using sum_type = std::conditional_t<
        std::is_integral<seq_t>::value && sizeof(seq_t) <= 8,
        unsigned __int128,
        seq_t>;
#else
    /* In C we always use a 128‑bit unsigned integer when available. */
    typedef unsigned __int128 sum_type;
#endif

    static seq_t *cache   = NULL; /* cached a(i) for 0 <= i < len */
    static idx_t len      = 0;    /* number of cached entries          */
    static size_t cap     = 0;    /* current capacity                  */
    static sum_type sum_prev = 0; /* Sum_{k<len} a(k)                  */

    if (n < len)
        return cache[(size_t)n];

    size_t needed = (size_t)(n + 1);

    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        assert(cache != NULL);
        cap   = needed;
    } else if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        assert(tmp != NULL);
        cache = tmp;
        cap   = needed;
    }

    for (idx_t i = len; i <= n; ++i) {
        seq_t val;
#if defined(__cplusplus)
        if constexpr (std::is_integral<sum_type>::value) {
            /* sum_type is 128‑bit; shift gives floor(sum/2) safely. */
            val = (seq_t)i + (seq_t)(sum_prev >> 1);
            sum_prev += (unsigned __int128)val;
        } else {
            /* sum_type is seq_t (e.g. arbitrary‑precision). */
            val = (seq_t)i + (seq_t)(sum_prev / 2);
            sum_prev += val;
        }
#else
        /* C: sum_type is 128‑bit. */
        val = (seq_t)i + (seq_t)(sum_prev >> 1);
        sum_prev += (unsigned __int128)val;
#endif
        cache[(size_t)i] = val;
    }

    len = (idx_t)needed;
    return cache[(size_t)n];
}
