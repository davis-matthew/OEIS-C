/*
 * A233246  Sum of squares of cycle lengths for different cycles in
 *          Fibonacci-like sequences modulo n.
 * https://oeis.org/A233246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{(x,y)\in(\mathbb{Z}/n)^2} \ell(x,y) where \ell(x,y)
 *            is the length of the cycle of the Fibonacci‑like recurrence
 *            modulo n, namely the smallest k>0 with (x_{k-2},x_{k-1}) = (x,
 *            y), x_{k} = x_{k-1}+x_{k-2} \pmod n
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used, but safe */
    if (n == 1) return 1;          /* one state (0,0) of length 1 */

    idx_t mod = n;
    idx_t total = mod * mod;               /* number of states */
    size_t words = (total + 63) >> 6;      /* number of 64‑bit words */
    uint64_t *visited = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!visited) return 0;               /* out of memory */

    seq_t result = 0;

    for (idx_t x = 0; x < mod; ++x) {
        for (idx_t y = 0; y < mod; ++y) {
            idx_t idx = x * mod + y;
            size_t word = idx >> 6;
            uint64_t mask = 1ULL << (idx & 63);
            if (visited[word] & mask) continue;   /* already processed */

            /* traverse the cycle starting from (x,y) */
            idx_t cx = x, cy = y;
            seq_t len = 0;
            while (1) {
                idx = cx * mod + cy;
                word = idx >> 6;
                mask = 1ULL << (idx & 63);
                if (visited[word] & mask) break;   /* cycle closed */

                visited[word] |= mask;
                ++len;

                /* next state */
                idx_t nx = cy;
                idx_t ny = cx + cy;
                if (ny >= mod) ny -= mod;          /* modular addition */
                cx = nx;
                cy = ny;
            }

            result += (len * len);
        }
    }

    free(visited);
    return result;
}
