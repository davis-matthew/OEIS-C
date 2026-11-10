/*
 * A069792  Triangular numbers in A062918.
 * https://oeis.org/A069792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th m = Sum_{i=1..k} reverse(i) (A062918(k)) that is
 *            triangular; found by incremental search over k
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 28, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-07)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isqrt_u128.h"

#define A069792_MAX 4096

static unsigned __int128 A069792_found[A069792_MAX];
static size_t A069792_cnt = 0;
static uint64_t A069792_k = 0;
static unsigned __int128 A069792_sum = 0;

static bool A069792_is_tri(unsigned __int128 s)
{
    unsigned __int128 t = 8 * s + 1;
    unsigned __int128 r = isqrt_u128(t);
    return r * r == t;
}

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    if (want > A069792_MAX) want = A069792_MAX;
    while (A069792_cnt < want) {
        A069792_k++;
        uint64_t x = A069792_k, rev = 0;
        while (x) {
            rev = rev * 10 + (x % 10);
            x /= 10;
        }
        A069792_sum += rev;
        if (A069792_is_tri(A069792_sum))
            A069792_found[A069792_cnt++] = A069792_sum;
    }
    return (seq_t)A069792_found[want - 1];
}
