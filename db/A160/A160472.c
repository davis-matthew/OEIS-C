/*
 * A160472  Fifth right hand column of triangles A094665 and A083061.
 * https://oeis.org/A160472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 496 a(n+1) =
 *            ((350*n^5+10745*n^4+130432*n^3+783031*n^2+2325618*n+2733984) *
 *            a(n)) / (175*n^4+3010*n^3+17441*n^2+38414*n+23808)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

static inline seq_t compute_next(idx_t n, seq_t prev)
{
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 m = n;
        unsigned __int128 m2 = m * m;
        unsigned __int128 m3 = m2 * m;
        unsigned __int128 m4 = m3 * m;
        unsigned __int128 m5 = m4 * m;
        unsigned __int128 num = 350 * m5 + 10745 * m4 + 130432 * m3 + 783031 * m2 + 2325618 * m + 2733984;
        unsigned __int128 den = 175 * m4 + 3010 * m3 + 17441 * m2 + 38414 * m + 23808;
        unsigned __int128 prod = (unsigned __int128)prev * num;
        return (seq_t)(prod / den);
    } else {
        seq_t m = (seq_t)n;
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t m4 = m3 * m;
        seq_t m5 = m4 * m;
        seq_t num = 350 * m5 + 10745 * m4 + 130432 * m3 + 783031 * m2 + 2325618 * m + 2733984;
        seq_t den = 175 * m4 + 3010 * m3 + 17441 * m2 + 38414 * m + 23808;
        return (prev * num) / den;
    }
#else
    unsigned __int128 m = n;
    unsigned __int128 m2 = m * m;
    unsigned __int128 m3 = m2 * m;
    unsigned __int128 m4 = m3 * m;
    unsigned __int128 m5 = m4 * m;
    unsigned __int128 num = 350 * m5 + 10745 * m4 + 130432 * m3 + 783031 * m2 + 2325618 * m + 2733984;
    unsigned __int128 den = 175 * m4 + 3010 * m3 + 17441 * m2 + 38414 * m + 23808;
    unsigned __int128 prod = (unsigned __int128)prev * num;
    return (seq_t)(prod / den);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)496;
    seq_t term = (seq_t)496;
    for (idx_t i = 0; i < n; ++i) {
        term = compute_next(i, term);
    }
    return term;
}
