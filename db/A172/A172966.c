/*
 * A172966  Number of ways to place 4 nonattacking knights on an n X n
 *          cylindrical board.
 * https://oeis.org/A172966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^7-54n^5+72n^4+1115n^3-2616n^2-8502n+26712)/24 for n
 *            >= 9. Brute‑force computation for n < 9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 24, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   knight_attacks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/knight_attacks.h"

seq_t a(idx_t n)
{
    /* Brute‑force for small boards (n < 9). */
    if (n < 9) {
        idx_t N = n * n;
        seq_t count = 0;
        for (idx_t i = 0; i < N; ++i) {
            idx_t r1 = i / n;
            idx_t c1 = i % n;
            for (idx_t j = i + 1; j < N; ++j) {
                idx_t r2 = j / n;
                idx_t c2 = j % n;
                if (knight_attacks(r1, c1, r2, c2, n)) continue;
                for (idx_t k = j + 1; k < N; ++k) {
                    idx_t r3 = k / n;
                    idx_t c3 = k % n;
                    if (knight_attacks(r1, c1, r3, c3, n) || knight_attacks(r2, c2, r3, c3, n))
                        continue;
                    for (idx_t l = k + 1; l < N; ++l) {
                        idx_t r4 = l / n;
                        idx_t c4 = l % n;
                        if (knight_attacks(r1, c1, r4, c4, n) ||
                            knight_attacks(r2, c2, r4, c4, n) ||
                            knight_attacks(r3, c3, r4, c4, n))
                            continue;
                        ++count;
                    }
                }
            }
        }
        return count;
    }

    /* Use the polynomial formula for n >= 9. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 n2  = N * N;
    unsigned __int128 n3  = n2 * N;
    unsigned __int128 n4  = n2 * n2;
    unsigned __int128 n5  = n3 * n2;
    unsigned __int128 n7  = n3 * n4;          /* N^3 * N^4 */
    unsigned __int128 poly =
        n7
        - (unsigned __int128)54 * n5
        + (unsigned __int128)72 * n4
        + (unsigned __int128)1115 * n3
        - (unsigned __int128)2616 * n2
        - (unsigned __int128)8502 * N
        + 26712;
    unsigned __int128 result = (N * poly) / 24;
    return (seq_t)result;
}
