/*
 * A007546  Number of steps to compute n-th prime in PRIMEGAME (fast version).
 * https://oeis.org/A007546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = Sum_{m=2..prime(n)} c(m), where c(m) is the exact number
 *            of PRIMEGAME steps spent on m. This comes from reading the
 *            program's loops as a register machine: for each m it runs 3m-1
 *            setup steps, then tests d = m-1, m-2, ... down to the largest
 *            proper divisor D of m (D = 1 when m is prime). Wi...
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 233, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 50 terms through n = 4688 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sum_{d=lo..hi} floor(m/d) */
static uint64_t floor_sum(uint64_t m, uint64_t lo, uint64_t hi) {
    uint64_t s = 0, d = lo;
    while (d <= hi) {
        uint64_t q = m / d;
        uint64_t last = m / q;
        if (last > hi) last = hi;
        s += q * (last - d + 1);
        d = last + 1;
    }
    return s;
}

static uint64_t cost_of(uint64_t m, uint64_t p) {
    uint64_t D = m / p;
    uint64_t c = 3 * m - 1;
    c += (m - 1 - D) * (6 * m + 2);
    if (D + 1 <= m - 1) c += 2 * floor_sum(m, D + 1, m - 1);
    c += 4 * m + 2 * (m / D) + 2;
    return c;
}

static uint64_t spf_u64(uint64_t m) {
    if (m % 2 == 0) return 2;
    for (uint64_t p = 3; p * p <= m; p += 2)
        if (m % p == 0) return p;
    return m;
}

seq_t a(idx_t n) {
    static idx_t cnt = 0;
    static uint64_t M = 1;
    static seq_t total = 0;

    if (n < cnt) {
        cnt = 0;
        M = 1;
        total = 0;
    }
    while (cnt < n) {
        M++;
        uint64_t p = spf_u64(M);
        total += (seq_t)cost_of(M, p);
        if (p == M) cnt++;
    }
    return total;
}
