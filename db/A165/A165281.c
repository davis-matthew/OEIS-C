/*
 * A165281  a(n) = (n+1)*(6*n^4 - 51*n^3 + 161*n^2 - 251*n + 251).
 * https://oeis.org/A165281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(6*n^4 - 51*n^3 + 161*n^2 - 251*n + 251)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 60, u64 n <= 4985,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Use signed __int128 for safe intermediate arithmetic */
    typedef __int128 int128_t;
    int128_t N = (int128_t)n;

    int128_t poly = (int128_t)6  * N * N * N * N
                  - (int128_t)51 * N * N * N
                  + (int128_t)161 * N * N
                  - (int128_t)251 * N
                  + (int128_t)251;

    poly *= (N + 1);

    assert(poly >= 0);            /* result must be non‑negative */
    return (seq_t)poly;
}
