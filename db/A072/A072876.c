/*
 * A072876  a(1) = a(2) = a(3) = a(4) = 1 and a(n) = (a(n-1)*a(n-3) +
 *          a(n-2)^3)/a(n-4) for n >= 5.
 * https://oeis.org/A072876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=a(2)=a(3)=a(4)=1; a(n) = (a(n-1)*a(n-3) + a(n-2)^3) /
 *            a(n-4) for n>=5
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, safe placeholder */

    /* --- static cache ----------------------------------------------------- */
    static seq_t *cache    = NULL;        /* array of computed terms          */
    static size_t capacity = 0;           /* allocated size of array          */
    static size_t computed = 0;           /* number of terms already set      */

    /* ensure array big enough for index n-1 (size_t is large enough) */
    if (capacity < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        if (cache == NULL) exit(1);        /* allocation failure -- abort */
        capacity = new_cap;
    }

    /* seed the first four terms when needed */
    if (computed < 4) {
        size_t up = (size_t)n < 4 ? (size_t)n : 4;
        for (size_t i = 0; i < up; ++i) cache[i] = (seq_t)1;
        computed = up;
    }

    /* compute remaining terms up to n */
    for (size_t idx = computed; idx < (size_t)n; ++idx) {
#if defined(__cplusplus)
        /* C++: use 128‑bit temporaries for integral types ≤ 64 bits */
        if constexpr (std::is_integral<seq_t>::value &&
                      sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 x1 = (unsigned __int128)cache[idx-1];
            unsigned __int128 x3 = (unsigned __int128)cache[idx-3];
            unsigned __int128 x2 = (unsigned __int128)cache[idx-2];
            unsigned __int128 denom = (unsigned __int128)cache[idx-4];
            unsigned __int128 numer = x1 * x3 + x2 * x2 * x2;
            cache[idx] = (seq_t)(numer / denom);
        } else {
            /* arbitrary‑precision class: rely on overloaded operators */
            seq_t prod13 = cache[idx-1] * cache[idx-3];
            seq_t prod22 = cache[idx-2] * cache[idx-2] * cache[idx-2];
            cache[idx] = (prod13 + prod22) / cache[idx-4];
        }
#else
        /* plain C: use unsigned __int128 for the multiplication */
        unsigned __int128 x1 = (unsigned __int128)cache[idx-1];
        unsigned __int128 x3 = (unsigned __int128)cache[idx-3];
        unsigned __int128 x2 = (unsigned __int128)cache[idx-2];
        unsigned __int128 denom = (unsigned __int128)cache[idx-4];
        unsigned __int128 numer = x1 * x3 + x2 * x2 * x2;
        cache[idx] = (seq_t)(numer / denom);
#endif
    }

    computed = (size_t)n;                 /* all terms up to n are cached */
    return cache[n-1];                    /* zero‑based array index   */
}
