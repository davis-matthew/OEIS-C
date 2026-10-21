/*
 * A078634  E.g.f. exp( 1 - x - sqrt( 1 - 4*x + x^2 )).
 * https://oeis.org/A078634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the recurrence a(n) +(-4*n+9)*a(n-1)
 *            +(n^2-16*n+26)*a(n-2) +(3*n^2-23*n+39)*a(n-3)
 *            +2*(n-3)*(n-4)*a(n-4)=0 with a(0)=1, a(1)=1, a(2)=4, a(3)=28
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes */
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/* Forward declarations for the helper used only in C++ mode */
#ifdef __cplusplus
/* Multiplication of an arbitrary‑precision seq_t by a small unsigned
   coefficient, using the sign of the coefficient when accumulating. */
static inline seq_t mul_abs(const seq_t &v, unsigned long long c) {
    /* Use 128‑bit intermediate for small integral seq_t types */
    if constexpr (std::is_integral_v<seq_t> && sizeof(seq_t) <= 8) {
        return (seq_t)((__int128)v * c);
    } else {
        return (seq_t)c * v;
    }
}
#endif

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* ------------------------------------------------------------------ */
/* Compute a(n) with memoisation; works for both C and C++.          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Resize cache if necessary */
    if (n + 1 > cache_len) {
        idx_t newlen = n + 1;
        seq_t *newc = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!newc) abort();
        /* Initialise new entries to 0 */
        for (idx_t i = cache_len; i < newlen; ++i) newc[i] = 0;
        cache = newc;
        cache_len = newlen;
    }

    /* Base cases */
    if (cache_len > 0 && cache[0] == 0) cache[0] = 1;
    if (cache_len > 1 && cache[1] == 0) cache[1] = 1;
    if (cache_len > 2 && cache[2] == 0) cache[2] = 4;
    if (cache_len > 3 && cache[3] == 0) cache[3] = 28;

    /* Compute missing terms */
    for (idx_t i = (cache_len > 4 ? 4 : cache_len); i <= n; ++i) {
#ifdef __cplusplus
        /* C++ mode – use arbitrary precision operations */
        seq_t pos = 0;
        seq_t neg = 0;

        long long c1 =  4 * (long long)i - 9;
        long long c2 = - (long long)i * (long long)i + 16 * (long long)i - 26;
        long long c3 = -3 * (long long)i * (long long)i + 23 * (long long)i - 39;
        long long c4 = -2 * (long long)(i - 3) * (long long)(i - 4);  /* always negative */

        if (c1 >= 0)
            pos += mul_abs(cache[i - 1], (unsigned long long)c1);
        else
            neg += mul_abs(cache[i - 1], (unsigned long long)(-c1));

        if (c2 >= 0)
            pos += mul_abs(cache[i - 2], (unsigned long long)c2);
        else
            neg += mul_abs(cache[i - 2], (unsigned long long)(-c2));

        if (c3 >= 0)
            pos += mul_abs(cache[i - 3], (unsigned long long)c3);
        else
            neg += mul_abs(cache[i - 3], (unsigned long long)(-c3));

        if (c4 >= 0)
            pos += mul_abs(cache[i - 4], (unsigned long long)c4);      /* never happens */
        else
            neg += mul_abs(cache[i - 4], (unsigned long long)(-c4));

        cache[i] = pos - neg;
#else
        /* C mode – use 128‑bit intermediates, safe for uint64_t builds */
        unsigned __int128 pos = 0;
        unsigned __int128 neg = 0;

        long long c1 =  4 * (long long)i - 9;
        long long c2 = - (long long)i * (long long)i + 16 * (long long)i - 26;
        long long c3 = -3 * (long long)i * (long long)i + 23 * (long long)i - 39;
        long long c4 = -2 * (long long)(i - 3) * (long long)(i - 4);  /* negative */

        if (c1 >= 0)
            pos += (unsigned __int128)cache[i - 1] * (unsigned long long)c1;
        else
            neg += (unsigned __int128)cache[i - 1] * (unsigned long long)(-c1);

        if (c2 >= 0)
            pos += (unsigned __int128)cache[i - 2] * (unsigned long long)c2;
        else
            neg += (unsigned __int128)cache[i - 2] * (unsigned long long)(-c2);

        if (c3 >= 0)
            pos += (unsigned __int128)cache[i - 3] * (unsigned long long)c3;
        else
            neg += (unsigned __int128)cache[i - 3] * (unsigned long long)(-c3);

        if (c4 >= 0)
            pos += (unsigned __int128)cache[i - 4] * (unsigned long long)c4;      /* never true */
        else
            neg += (unsigned __int128)cache[i - 4] * (unsigned long long)(-c4);

        cache[i] = (seq_t)(pos - neg);
#endif
    }
    return cache[n];
}
