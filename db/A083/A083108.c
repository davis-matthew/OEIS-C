/*
 * A083108  Product of terms in row n of A083106.
 * https://oeis.org/A083108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of row n of A083106 (row r = the r smallest
 *            distinct positive integers coprime to r whose product is 1 mod
 *            r); computed by construction: first r-1 coprimes plus a final
 *            adjusted element
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    uint64_t r = (uint64_t)n;
    uint64_t *v = (uint64_t *)malloc((size_t)r * sizeof(uint64_t));
    if (v == NULL) return (seq_t)0;

    uint64_t cnt = 0;
    uint64_t i = 1;
    while (cnt < r - 1) {
        if (gcd_u64(i, r) == 1) v[cnt++] = i;
        i++;
    }
    /* n-th coprime */
    while (gcd_u64(i, r) != 1) i++;
    uint64_t last = i;

    uint64_t p = 1;
    for (uint64_t k = 0; k < r - 1; k++) p = (p * (v[k] % r)) % r;
    uint64_t full = (p * (last % r)) % r;

    if (full != 1) {
        uint64_t inv = 1;
        for (uint64_t t = 1; t < r; t++) {
            if ((t * p) % r == 1) { inv = t; break; }
        }
        uint64_t cand = inv;
        for (;;) {
            bool used = false;
            for (uint64_t k = 0; k < r - 1; k++) {
                if (v[k] == cand) { used = true; break; }
            }
            if (!used) break;
            cand += r;
        }
        last = cand;
    }

    seq_t res = (seq_t)1;
    for (uint64_t k = 0; k < r - 1; k++) res = res * (seq_t)v[k];
    res = res * (seq_t)last;
    free(v);
    return res;
}
