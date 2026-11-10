/*
 * A070873  Integers k such that A070871(k) > k.
 * https://oeis.org/A070873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer k with A070871(k) > k, where A070871(k) =
 *            A002487(k)*A002487(k+1); found by a sequential search with
 *            memoization
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t fusc_a070873(uint64_t n)
{
    uint64_t a = 1, b = 0;
    while (n) {
        if (n & 1) b += a; else a += b;
        n >>= 1;
    }
    return b;
}

static uint64_t *terms_a070873 = NULL;
static idx_t count_a070873 = 0, cap_a070873 = 0;
static uint64_t next_k_a070873 = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (count_a070873 < n) {
        uint64_t k = next_k_a070873++;
        uint64_t x = fusc_a070873(k);
        uint64_t y = fusc_a070873(k + 1);
        if (y != 0 && x > k / y) {
            if (count_a070873 == cap_a070873) {
                idx_t nc = cap_a070873 ? cap_a070873 * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(terms_a070873, (size_t)nc * sizeof(uint64_t));
                if (!t) { next_k_a070873--; return 0; }
                terms_a070873 = t;
                cap_a070873 = nc;
            }
            terms_a070873[count_a070873++] = k;
        }
    }
    return (seq_t)terms_a070873[n - 1];
}
