/*
 * A107970  a(n) = (n+1)*(n+2)^3*(n+3)*(2n+3)*(2n+5)/360.
 * https://oeis.org/A107970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^3*(n+3)*(2n+3)*(2n+5)/360
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * The following definition uses a 128‑bit intermediate when available;
 * otherwise it performs factor cancelation to avoid overflow.
 */
seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    /* 128‑bit intermediate */
    unsigned __int128 r = 1;

    r *= (unsigned __int128)(n + 1);

    uint64_t np2 = (uint64_t)n + 2;
    r *= (unsigned __int128)np2 * np2 * np2;          /* (n+2)^3 */

    r *= (unsigned __int128)(n + 3);
    r *= (unsigned __int128)(2 * n + 3);
    r *= (unsigned __int128)(2 * n + 5);

    r /= 360;

    return (seq_t)r;
#else
    /* Cancel factors with denominator to stay within seq_t limits */
    seq_t res = 1;
    seq_t d   = 360;

    /* helper to multiply and reduce with the remaining denominator */
    auto process = [&](seq_t mult) {
        res *= mult;
        seq_t g = seq_gcd(res, d);
        if (g != 1) {
            res /= g;
            d   /= g;
        }
    };

    process((seq_t)(n + 1));

    seq_t np2 = (seq_t)(n + 2);
    seq_t cube = np2 * np2 * np2;
    process(cube);

    process((seq_t)(n + 3));
    process((seq_t)(2 * n + 3));
    process((seq_t)(2 * n + 5));

    /* Any remaining denominator should be 1 */
    if (d != 1) res /= d;

    return res;
#endif
}
