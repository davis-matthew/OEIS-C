/*
 * A232498  Primes in the tribonacci-like sequence, A020992.
 * https://oeis.org/A232498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes in A020992 (b(k) = b(k-1) + b(k-2) + b(k-3), b(0)=0,
 *            b(1)=2, b(2)=1), in order; search using a Miller-Rabin test
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   is_prime_u64, mem, mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod_seq.h"

static seq_t A232498_powmod(seq_t b, seq_t e, seq_t m) {
    seq_t r = (seq_t)1;
    b = b % m;
    while (e > 0) {
        if ((e & 1) != 0) r = mulmod_seq(r, b, m);
        e >>= 1;
        if (e > 0) b = mulmod_seq(b, b, m);
    }
    return r;
}

static bool A232498_is_prime(seq_t x) {
    static const unsigned small[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    static const unsigned bases[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    const seq_t lim64 = (seq_t)UINT64_MAX;
    if (x <= lim64) return is_prime_u64((uint64_t)x);
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); i++)
        if (x % small[i] == 0) return false;
    seq_t xm1 = x - 1;
    seq_t d = xm1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); i++) {
        seq_t y = A232498_powmod((seq_t)bases[i], d, x);
        if (y == 1 || y == xm1) continue;
        bool comp = true;
        for (unsigned r = 1; r < s; r++) {
            y = mulmod_seq(y, y, x);
            if (y == xm1) { comp = false; break; }
            if (y == 1) break;
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static seq_t *found = NULL;
    static size_t cap = 0;
    static size_t cnt = 0;
    static seq_t x, y, z;
    static bool init = false;
    static bool overflow = false;

    if (!init) {
        cap = 16;
        found = mem_new(seq_t, cap);
        if (!found) return (seq_t)0;
        found[0] = (seq_t)2;   /* b(1) = 2 is prime; b(0)=0, b(2)=1 are not */
        cnt = 1;
        x = (seq_t)0;
        y = (seq_t)2;
        z = (seq_t)1;
        init = true;
    }

    while ((idx_t)cnt < n && !overflow) {
        seq_t t = x + y;
        if (t < x) { overflow = true; break; }
        seq_t s = t + z;
        if (s < z) { overflow = true; break; }
        x = y;
        y = z;
        z = s;
        if (A232498_is_prime(s)) {
            if (cnt >= cap) {
                size_t nc = cap * 2;
                seq_t *tmp = mem_resize(seq_t, found, nc);
                if (!tmp) return (seq_t)0;
                found = tmp;
                cap = nc;
            }
            found[cnt++] = s;
        }
    }

    if (n >= 1 && (idx_t)cnt >= n) return found[(size_t)(n - 1)];
    return (seq_t)0;
}
