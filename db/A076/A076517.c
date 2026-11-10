/*
 * A076517  Partial sums of A060370 (cf. A006556).
 * https://oeis.org/A076517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (p_k-1)/ord_{p_k}(10), where p_k is the k-th
 *            prime >= 7 (A060370 terms); computed by prime search and
 *            multiplicative order
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A076517_mulmod(uint64_t x, uint64_t y, uint64_t m) {
    return (uint64_t)((unsigned __int128)x * y % m);
}

static uint64_t A076517_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A076517_mulmod(r, b, m);
        b = A076517_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static uint64_t A076517_ratio(uint64_t p) {
    uint64_t phi = p - 1, order = phi, t = phi;
    for (uint64_t q = 2; q * q <= t; q++) {
        if (t % q == 0) {
            while (t % q == 0) t /= q;
            while (order % q == 0 && A076517_powmod(10, order / q, p) == 1)
                order /= q;
        }
    }
    if (t > 1) {
        uint64_t q = t;
        while (order % q == 0 && A076517_powmod(10, order / q, p) == 1)
            order /= q;
    }
    return phi / order;
}

seq_t a(idx_t n) {
    static idx_t cn = 0;
    static uint64_t cp = 5;
    static seq_t csum = 0;
    if (n < cn || cn == 0) {
        cn = 0; cp = 5; csum = 0;
    }
    while (cn < n) {
        do { cp++; } while (!is_prime_u64(cp) || cp < 7);
        csum += (seq_t)A076517_ratio(cp);
        cn++;
    }
    return csum;
}
