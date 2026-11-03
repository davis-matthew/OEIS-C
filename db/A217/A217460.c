/*
 * A217460  Odd values of n such that the polynomial 1+x+x^2+...+x^(n-1) is
 *          reducible over GF(2).
 * https://oeis.org/A217460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 93 terms through n = 889 (2026-11-03)
 * Helpers:   is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217460: a(n) = the nth odd integer n for which
   1+x+...+x^(n-1) is reducible over GF(2).  Equivalently,
   all odd composite numbers plus primes for which 2 is NOT a
   primitive root modulo the prime.  The function implements this
   definition by enumerating odd integers, testing primality and,
   for primes, testing the primitive‑root condition. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

/* Return true iff 2 is a primitive root modulo the odd prime p. */
static bool is_primitive_root_2_prime(idx_t p)
{
    /* p is assumed prime and odd */
    idx_t phi   = p - 1;   /* Euler's totient of p */
    idx_t temp  = phi;     /* a copy for factorisation */
    idx_t factors[64];
    size_t cnt = 0;

    /* factorise phi into distinct prime factors */
    for (idx_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            factors[cnt++] = d;
            while (temp % d == 0)
                temp /= d;
        }
    }
    if (temp > 1)
        factors[cnt++] = temp;   /* remaining prime factor */

    /* test the primitive‑root condition:  for each prime factor q
       of phi, 2^(phi/q) must not be congruent to 1 mod p. */
    for (size_t i = 0; i < cnt; ++i) {
        idx_t q = factors[i];
        seq_t r = powmod((seq_t)2, (uint64_t)(phi / q), (seq_t)p);
        if (r == (seq_t)1)
            return false;       /* 2 is not primitive */
    }
    return true;                /* 2 is primitive */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined, but harmless */

    idx_t count = 0;

    for (idx_t m = 3; ; m += 2) {    /* iterate over odd integers > 1 */
        bool include;
        if (is_prime_big((seq_t)m))
            include = !is_primitive_root_2_prime(m);
        else
            include = true;          /* odd composite numbers */

        if (include) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
