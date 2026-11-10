/*
 * A075762  A001003(p-1)/p where p runs through the odd primes.
 * https://oeis.org/A075762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001003(prime(n+1)-1)/prime(n+1), where (k+1)*A001003(k)
 *            = (6*k-3)*A001003(k-1) - (k-2)*A001003(k-2), A001003(0) =
 *            A001003(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static unsigned __int128 A001003_u128(uint64_t n) {
    unsigned __int128 x2 = 1, x1 = 1;
    uint64_t i;
    if (n <= 1) return 1;
    for (i = 2; i <= n; i++) {
        uint64_t d = i + 1;
        unsigned __int128 q1 = x1 / d;
        unsigned __int128 q2 = x2 / d;
        long long r1 = (long long)(x1 % d);
        long long r2 = (long long)(x2 % d);
        long long t = (long long)(6 * i - 3) * r1 - (long long)(i - 2) * r2;
        long long tq = t / (long long)d;
        unsigned __int128 v = q1 * (6 * i - 3);
        if (tq > 0) v += (unsigned __int128)(unsigned long long)tq;
        v -= q2 * (i - 2);
        if (tq < 0) v -= (unsigned __int128)(unsigned long long)(-tq);
        x2 = x1;
        x1 = v;
    }
    return x1;
}

static seq_t A001003_seq(idx_t n) {
    seq_t x2 = 1;
    seq_t x1 = 1;
    idx_t i;
    if (n <= 1) return 1;
    for (i = 2; i <= n; i++) {
        idx_t d = i + 1;
        seq_t q1 = x1 / (seq_t)d;
        seq_t q2 = x2 / (seq_t)d;
        long long r1 = (long long)(x1 % (seq_t)d);
        long long r2 = (long long)(x2 % (seq_t)d);
        long long t = (long long)(6 * i - 3) * r1 - (long long)(i - 2) * r2;
        long long tq = t / (long long)d;
        seq_t v = q1 * (seq_t)(6 * i - 3);
        if (tq > 0) v += (seq_t)(unsigned long long)tq;
        v -= q2 * (seq_t)(i - 2);
        if (tq < 0) v -= (seq_t)(unsigned long long)(-tq);
        x2 = x1;
        x1 = v;
    }
    return x1;
}

static uint64_t A075762_prime(idx_t k) {
    /* k-th prime, 1-indexed, by trial division */
    uint64_t c = 1;
    idx_t cnt = 0;
    while (cnt < k) {
        uint64_t j;
        int ok;
        c++;
        ok = 1;
        for (j = 2; j * j <= c; j++) {
            if (c % j == 0) { ok = 0; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

seq_t a(idx_t n) {
    uint64_t p = A075762_prime(n + 1);
    if (p - 1 <= 54) {
        unsigned __int128 v = A001003_u128(p - 1) / p;
        seq_t r = 0;
        int s;
        for (s = 3; s >= 0; s--) {
            uint32_t limb = (uint32_t)(v >> (32 * s));
            r = r * (seq_t)65536 * (seq_t)65536 + (seq_t)limb;
        }
        return r;
    }
    return A001003_seq((idx_t)(p - 1)) / (seq_t)p;
}
