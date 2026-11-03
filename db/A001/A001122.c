/*
 * A001122  Primes with primitive root 2.
 * https://oeis.org/A001122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search over primes p (odd p only), keeping p when 2 has
 *            multiplicative order p-1 mod p, i.e. 2^((p-1)/q) != 1 (mod p)
 *            for every prime q | p-1
 * Range:     u8 n <= 24, u16 n <= 2443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9943 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static bool a1122_prim2(uint64_t p) {
    uint64_t phi = p - 1, t = phi;
    for (uint64_t q = 2; q * q <= t; q++) {
        if (t % q == 0) {
            if (powmod_u64(2, phi / q, p) == 1) return false;
            while (t % q == 0) t /= q;
        }
    }
    if (t > 1 && powmod_u64(2, phi / t, p) == 1) return false;
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 1;
    while (count < n) {
        p += 2;
        if (is_prime_u64(p) && a1122_prim2(p)) count++;
    }
    return (seq_t)p;
}
