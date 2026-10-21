/*
 * A064286  Number of cyclotomic cosets C of 2 mod 2n+1 such that -C = C.
 * https://oeis.org/A064286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = number of cyclotomic cosets C of 2 modulo 2n+1 such that
 *            -C = C
 * Range:     u8 n <= 4095, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* modulus M = 2n + 1 (odd, >1 for n>=1) */
    idx_t M = 2 * n + 1;

    /* orbit[i] = id of the orbit containing i; -1 means unvisited */
    int32_t *orbit = (int32_t *)malloc((size_t)M * sizeof(int32_t));
    if (!orbit) return 0;          /* allocation failure: return 0 */
    for (idx_t i = 0; i < M; ++i) orbit[i] = -1;

    seq_t count = 0;           /* number of symmetric cosets */
    int32_t orbit_id = 0;      /* current orbit id */

    for (idx_t start = 1; start < M; ++start) {
        if (orbit[start] != -1) continue;   /* already assigned */

        /* build the orbit of 'start' under multiplication by 2 modulo M */
        idx_t x = start;
        do {
            orbit[x] = orbit_id;
            x = (x * 2) % M; /* since x < M and M <= 2n+1 < 2^64, this is safe */
        } while (x != start);

        /* test whether this orbit is symmetric: -C = C */
        bool symmetric = true;
        x = start;
        do {
            idx_t neg = (M - x) % M;   /* additive inverse of x modulo M */
            if (orbit[neg] != orbit_id) { symmetric = false; break; }
            x = (x * 2) % M;
        } while (x != start);

        if (symmetric) ++count;
        ++orbit_id;
    }

    free(orbit);
    return count;
}
