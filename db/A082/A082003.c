/*
 * A082003  First row of array in A082002.
 * https://oeis.org/A082003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(1,n) of A082002; computed by simulating the array
 *            (search with gcd constraints), antidiagonals alternating
 *            direction
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint64_t A082003_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    size_t N = (size_t)n;
    size_t w = N + 2;
    uint64_t *table = (uint64_t *)calloc(w * w, sizeof(uint64_t));
    size_t cap = 1024;
    uint8_t *used = (uint8_t *)calloc(cap, 1);
    if (!table || !used) { free(table); free(used); return (seq_t)0; }
    table[1 * w + 1] = 1;
    used[1] = 1;
    uint64_t result = 0;
    for (size_t d = 2; d <= N; ++d) {
        for (size_t s = 1; s <= d; ++s) {
            size_t i = (d % 2 == 0) ? s : (d - s + 1);
            size_t j = d - i + 1;
            uint64_t cand = 2;
            for (;;) {
                if (cand >= cap) {
                    size_t nc = cap;
                    while (nc <= cand) nc <<= 1;
                    uint8_t *t = (uint8_t *)realloc(used, nc);
                    if (!t) { free(table); free(used); return (seq_t)0; }
                    memset(t + cap, 0, nc - cap);
                    used = t;
                    cap = nc;
                }
                if (used[cand]) { ++cand; continue; }
                bool ok = true;
                for (size_t k = 1; k < j && ok; ++k) {
                    uint64_t v = table[i * w + k];
                    if (v > 1 && A082003_gcd(cand, v) == 1) ok = false;
                }
                for (size_t r = 1; r < i && ok; ++r) {
                    uint64_t v = table[r * w + j];
                    if (v > 1 && A082003_gcd(cand, v) == 1) ok = false;
                }
                if (ok) break;
                ++cand;
            }
            table[i * w + j] = cand;
            used[cand] = 1;
            if (i == 1 && j == N) result = cand;
        }
    }
    free(table);
    free(used);
    return (seq_t)result;
}
