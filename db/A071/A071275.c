/*
 * A071275  Palindromes in A071274.
 * https://oeis.org/A071275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th term, in increasing order of m, of the palindromes
 *            among A071274(m) = (reverse(m) concatenated with m) / 11. This
 *            is a search over m, caching results
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A071275_cache = 0;
static size_t A071275_cnt = 0, A071275_cap = 0;
static uint64_t A071275_next_m = 1;

static bool A071275_is_pal(uint64_t v)
{
    uint64_t r = 0, t = v;
    while (t) { r = r * 10 + t % 10; t /= 10; }
    return r == v;
}

static uint64_t A071275_val(uint64_t m)
{
    uint64_t rev = 0, t = m, p = 1;
    while (t) { rev = rev * 10 + t % 10; t /= 10; p *= 10; }
    return (rev * p + m) / 11;
}

seq_t a(idx_t n)
{
    while (A071275_cnt < (size_t)n) {
        uint64_t m = A071275_next_m++;
        uint64_t v = A071275_val(m);
        if (A071275_is_pal(v)) {
            if (A071275_cnt == A071275_cap) {
                A071275_cap = A071275_cap ? A071275_cap * 2 : 1024;
                A071275_cache = (uint64_t *)realloc(A071275_cache, A071275_cap * sizeof(uint64_t));
            }
            A071275_cache[A071275_cnt++] = v;
        }
    }
    return (seq_t)A071275_cache[n - 1];
}
