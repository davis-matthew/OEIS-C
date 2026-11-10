/*
 * A066047  Numbers k that divide A001045(k-1).
 * https://oeis.org/A066047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 2 such that k | A001045(k-1), where A001045(m)
 *            = (2^m - (-1)^m)/3; i.e. 3k | 2^(k-1) - (-1)^(k-1). Computed by
 *            a search with modular exponentiation
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 849 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A066047_powmod(uint64_t b, uint64_t e, uint64_t m) {
    unsigned __int128 r = 1, x = b % m;
    while (e) {
        if (e & 1) r = (r * x) % m;
        x = (x * x) % m;
        e >>= 1;
    }
    return (uint64_t)r;
}

seq_t a(idx_t n) {
    uint64_t k = 1;
    idx_t cnt = 0;
    while (cnt < n) {
        k++;
        uint64_t m = k - 1;
        uint64_t M = 3 * k;
        uint64_t r = A066047_powmod(2, m, M);
        int ok;
        if (m & 1) ok = (r == M - 1);
        else ok = (r == 1);
        if (ok) cnt++;
    }
    return (seq_t)k;
}
