/*
 * A049587  Primes p such that x^55 = 2 has a solution mod p.
 * https://oeis.org/A049587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that x^55 ≡ 2 (mod p) has a
 *            solution. A prime p (including 2) satisfies the condition iff
 *            2^((p-1)/gcd(p-1,55)) ≡ 1 (mod p). This check is performed for
 *            each candidate prime until the nth such prime is found
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 964 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/powmod.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static bool satisfies(uint64_t p)
{
    if (p == 2) return true;
    uint64_t g = gcd_u64(p - 1, 55ULL);
    uint64_t exp = (p - 1) / g;
    return powmod((seq_t)2, exp, (seq_t)p) == (seq_t)1;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t candidate = 2;

    while (1) {
        if (candidate == 2 || candidate % 2 == 1) {
            if (is_prime_big((seq_t)candidate)) {
                if (satisfies(candidate)) {
                    found++;
                    if (found == n) {
                        return (seq_t)candidate;
                    }
                }
            }
        }
        candidate++;
    }
}
