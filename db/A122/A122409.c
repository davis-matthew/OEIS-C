/*
 * A122409  Numbers k such that there is no cube between k^2 and (k+1)^2.
 * https://oeis.org/A122409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122409: numbers k such that no cube lies strictly between k² and (k+1)².
   Algorithm: enumerate all cubes m³, for each cube compute floor(sqrt(m³)).
   If m³ is not a perfect square, the corresponding k is excluded.
   After marking all excluded k up to a limit large enough to contain the first n
   non‑excluded numbers, scan from 1 upward to find the n‑th non‑excluded k. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* integer square‑root: floor(sqrt(n)) for a 64‑bit unsigned integer. */
static idx_t
isqrt_u64(uint64_t n)
{
    idx_t x = 0;
    for (int i = 62; i >= 0; --i) {        /* 2^32–1 < 2^63, so i=62 suffices */
        idx_t y = x | ((idx_t)1 << i);
        if ((unsigned __int128)y * y <= n) {
            x = y;
        }
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; 0 is never used */

    /* Initial guess for the search limit; will be doubled if insufficient. */
    idx_t limit = n + 100;
    if (limit < 10) limit = 10;

    while (1) {
        bool *excluded = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!excluded) return 0;   /* unlikely, but keep behaviour defined */
        for (idx_t i = 0; i <= limit; ++i) excluded[i] = false;

        /* Mark excluded k's from cubes m³. */
        for (idx_t m = 2;; ++m) {
            uint64_t cube = (uint64_t)m * m * m;
            idx_t k = isqrt_u64(cube);
            if (k > limit) break;
            if ((uint64_t)k * k != cube) {   /* cube strictly between squares */
                excluded[k] = true;
            }
        }

        /* Scan for the n‑th non‑excluded k. */
        idx_t cnt = 0;
        seq_t result = 0;
        for (idx_t k = 1; k <= limit; ++k) {
            if (!excluded[k]) {
                ++cnt;
                if (cnt == n) {
                    result = k;
                    free(excluded);
                    return result;
                }
            }
        }
        free(excluded);
        /* Not enough terms yet – enlarge the limit and retry. */
        limit *= 2;
        if (limit < n) limit = n;  /* just in case */
    }
}
