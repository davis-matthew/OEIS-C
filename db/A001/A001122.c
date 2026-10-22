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
 * Helpers:   a1122_isprime, a1122_mulmod, a1122_powmod, a1122_prim2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a1122_isprime.h"
#include "../utils/a1122_mulmod.h"
#include "../utils/a1122_powmod.h"
#include "../utils/a1122_prim2.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 1;
    while (count < n) {
        p += 2;
        if (a1122_isprime(p) && a1122_prim2(p)) count++;
    }
    return (seq_t)p;
}
