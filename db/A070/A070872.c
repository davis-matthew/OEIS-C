/*
 * A070872  Numbers k such that A070871(k) = k.
 * https://oeis.org/A070872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k >= 1 with A002487(k)*A002487(k+1) = k; found by a
 *            search over k using s(h*2^16+l) = s(h)*s(2^16-l) + s(h+1)*s(l)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A070872_M 65536u

static uint32_t A070872_T[A070872_M + 1];
static bool A070872_init = false;
static uint64_t A070872_found[1024];
static unsigned A070872_cnt = 0;
static uint64_t A070872_h = 0; /* next block to scan */

static uint64_t A070872_fusc(uint64_t n)
{
    uint64_t x = 1, y = 0;
    while (n) {
        if (n & 1) y += x; else x += y;
        n >>= 1;
    }
    return y;
}

static void A070872_setup(void)
{
    uint32_t i;
    A070872_T[0] = 0;
    A070872_T[1] = 1;
    for (i = 2; i <= A070872_M; i++) {
        if ((i & 1u) == 0) A070872_T[i] = A070872_T[i >> 1];
        else A070872_T[i] = A070872_T[i >> 1] + A070872_T[(i >> 1) + 1];
    }
    A070872_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A070872_init) A070872_setup();
    while (A070872_cnt < n && A070872_cnt < 1024 && A070872_h < (1ULL << 32)) {
        uint64_t h = A070872_h++;
        uint64_t sh = A070872_fusc(h), sh1 = A070872_fusc(h + 1);
        uint64_t base = h * (uint64_t)A070872_M;
        uint32_t l = (h == 0) ? 1u : 0u;
        for (; l < A070872_M; l++) {
            uint64_t s0 = sh * A070872_T[A070872_M - l] + sh1 * A070872_T[l];
            uint64_t s1 = sh * A070872_T[A070872_M - l - 1] + sh1 * A070872_T[l + 1];
            uint64_t k = base + l;
            if (s0 * s1 == k) {
                if (A070872_cnt < 1024) A070872_found[A070872_cnt++] = k;
            }
        }
    }
    if (n > A070872_cnt) return 0;
    return (seq_t)A070872_found[n - 1];
}
