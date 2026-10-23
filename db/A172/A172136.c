/*
 * A172136  Number of ways to place 5 nonattacking knights on an n X n board.
 * https://oeis.org/A172136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 - 90*n^8 + 240*n^7 + 3235*n^6 - 16320*n^5 -
 *            40530*n^4 * + 396480*n^3 - 231656*n^2 - 3359520*n + 6509280)/120
 *            (n ≥ 8). * For n < 8 compute by brute force enumeration
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 15, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   knights_attack
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/knights_attack.h"

/*   a(n)   Number of ways to place 5 non‑attacking knights on an n×n board */
seq_t a(idx_t n)
{
    /* brute force for small boards – the polynomial is not useful for n < 8 */
    if (n < 8) {
        idx_t N = n * n;
        seq_t total = 0;
        for (idx_t a = 0; a + 4 < N; ++a)
            for (idx_t b = a + 1; b + 3 < N; ++b)
                if (!knights_attack(a, b, n))
                for (idx_t c = b + 1; c + 2 < N; ++c)
                    if (!knights_attack(a, c, n) && !knights_attack(b, c, n))
                    for (idx_t d = c + 1; d + 1 < N; ++d)
                        if (!knights_attack(a, d, n) && !knights_attack(b, d, n) &&
                            !knights_attack(c, d, n))
                        for (idx_t e = d + 1; e < N; ++e)
                            if (!knights_attack(a, e, n) && !knights_attack(b, e, n) &&
                                !knights_attack(c, e, n) && !knights_attack(d, e, n))
                                ++total;
        return total;
    }

    /* --------------------------------------------------------------- */
    /*  Polynomial evaluation using signed 128‑bit intermediate.      */
    /* --------------------------------------------------------------- */
#if defined(__INT128__) || defined(__SIZEOF_INT128__)
    __int128 nn = n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;
    __int128 n6 = n5 * nn;
    __int128 n7 = n6 * nn;
    __int128 n8 = n7 * nn;
    __int128 n9 = n8 * nn;
    __int128 n10 = n9 * nn;

    __int128 num =         n10
                     - (__int128)90 * n8
                     + (__int128)240 * n7
                     + (__int128)3235 * n6
                     - (__int128)16320 * n5
                     - (__int128)40530 * n4
                     + (__int128)396480 * n3
                     - (__int128)231656 * n2
                     - (__int128)3359520 * nn
                     + (__int128)6509280;

    return (seq_t)(num / (__int128)120);
#else
    /* Fallback path: use seq_t arithmetic (uses arbitrary precision if available) */
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;
    seq_t n4 = n3 * tn;
    seq_t n5 = n4 * tn;
    seq_t n6 = n5 * tn;
    seq_t n7 = n6 * tn;
    seq_t n8 = n7 * tn;
    seq_t n9 = n8 * tn;
    seq_t n10 = n9 * tn;

    seq_t num = n10;
    num -= (seq_t)90 * n8;
    num += (seq_t)240 * n7;
    num += (seq_t)3235 * n6;
    num -= (seq_t)16320 * n5;
    num -= (seq_t)40530 * n4;
    num += (seq_t)396480 * n3;
    num -= (seq_t)231656 * n2;
    num -= (seq_t)3359520 * tn;
    num += (seq_t)6509280;

    return num / (seq_t)120;
#endif
}
