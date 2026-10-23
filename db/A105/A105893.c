/*
 * A105893  Primes for which -21 is a primitive root.
 * https://oeis.org/A105893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   prime p such that −21 is a primitive root modulo p (OEIS
 *            A105893)
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   mod_neg21, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_neg21.h"
#include "../utils/mul_mod64.h"

/* Helper for modular multiplication (avoids overflow using 128-bit) */

/* Helper for modular exponentiation */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; i++) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* Bases sufficient for n < 2^64 (Deterministic variant) */
    static const uint64_t test_bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof test_bases / sizeof test_bases[0]; i++) {
        uint64_t a = test_bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Test whether −21 is a primitive root modulo prime p */
static bool primitive_minus21(uint64_t p)
{
    if (p == 2) return true;      /* the only unit is 1 */
    uint64_t g = mod_neg21(p);
    if (g == 0) return false;     /* p divides 21 */

    uint64_t m = p - 1;
    uint64_t tmp = m;
    /* distinct prime factors of p-1 */
    uint64_t factors[32];
    size_t nf = 0;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            factors[nf++] = d;
            while (tmp % d == 0) tmp /= d;
        }
    }
    if (tmp > 1) factors[nf++] = tmp;

    for (size_t i = 0; i < nf; ++i) {
        uint64_t q = factors[i];
        if (mod_pow64(g, m / q, p) == 1ULL)
            return false;
    }
    return true;
}

/* Sequence A105893 */
seq_t a(idx_t n)
{
    /* static state: list of found primes satisfying the condition */
    static seq_t *found = NULL;
    static idx_t sz = 0, cap = 0;
    static uint64_t cand = 2;           /* next candidate to test */

    while (sz < n) {
        uint64_t p = cand;
        if (is_prime_uint64(p) && primitive_minus21(p)) {
            if (sz == cap) {
                cap = (cap == 0 ? 4 : cap * 2);
                found = (seq_t*)realloc(found, cap * sizeof(seq_t));
            }
            found[sz++] = (seq_t)p;
        }
        /* Advance to next candidate: after 2 only odd numbers */
        cand += (cand == 2 ? 1 : 2);
    }
    return found[n - 1];
}
