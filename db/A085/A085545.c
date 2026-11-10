/*
 * A085545  Smaller of number pairs from A054038 which together use all ten
 *          digits once.
 * https://oeis.org/A085545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, base, nonn
 * Formula:   search -- a(n) = n-th smallest k such that k and k^2 qualify in
 *            A054038 (k^2 has all ten digits), k has 5 distinct digits, and
 *            some larger A054038 term has the remaining 5 digits
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t A085545_list[100000];
static unsigned A085545_cnt = 0;
static bool A085545_ready = false;

static unsigned A085545_digmask(uint64_t v)
{
    unsigned m = 0;
    if (v == 0) return 1u;
    while (v > 0) {
        m |= 1u << (unsigned)(v % 10);
        v /= 10;
    }
    return m;
}

static void A085545_init(void)
{
    static uint32_t maxk[1024];
    static unsigned kmask[100000];
    uint32_t k;
    unsigned i;
    for (i = 0; i < 1024; i++) maxk[i] = 0;
    for (k = 10000; k < 100000; k++) {
        uint64_t sq = (uint64_t)k * k;
        unsigned m = A085545_digmask(k);
        kmask[k] = 0;
        if (__builtin_popcount(m) != 5) continue;
        if (A085545_digmask(sq) != 1023u) continue;
        kmask[k] = m;
        if (k > maxk[m]) maxk[m] = k;
    }
    for (k = 10000; k < 100000; k++) {
        unsigned m = kmask[k];
        if (!m) continue;
        if (maxk[1023u ^ m] > k) A085545_list[A085545_cnt++] = k;
    }
    A085545_ready = true;
}

seq_t a(idx_t n)
{
    if (!A085545_ready) A085545_init();
    if (n == 0 || n > (idx_t)A085545_cnt) return 0;
    return (seq_t)A085545_list[n - 1];
}
