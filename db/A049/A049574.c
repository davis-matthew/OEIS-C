/*
 * A049574  Primes p such that x^42 = 2 has a solution mod p.
 * https://oeis.org/A049574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 998 (2026-11-03)
 * Helpers:   gcd_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Criterion: 2 is a 42nd power residue modulo p.
 * For a prime p > 2, the congruence x^42 ≡ 2 (mod p) has a solution iff
 *      2^((p‑1)/gcd(42, p‑1)) ≡ 1 (mod p).
 *  p = 2 satisfies the congruence by definition (0^42 ≡ 2 (mod 2)).
 * This function returns the n‑th prime that satisfies the above condition
 * (offset 1 as in OEIS).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd_i32.h"
#include "../utils/mem.h"
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

/* Small integer gcd. */

/* Binary modular exponentiation that works for any seq_t size. */
static inline seq_t mod_pow_big(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    while (exp != 0) {
        if ((exp & 1) != 0)
            result = (result * b) % mod;
        exp = exp >> 1;
        if (exp != 0)
            b = (b * b) % mod;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence starts at index 1 */

    static seq_t *primes = NULL;   /* cache of found primes */
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t last_checked = 1;   /* prime predecessor of the first prime */

    if (primes == NULL) {
        cap = 16;
        primes = mem_new(seq_t, cap);
    }

    seq_t candidate = last_checked + 1;
    if (candidate == 1)          /* ensure we start at 2 */
        candidate = 2;

    while (len < n) {
        /* Make candidate odd after the prime 2. */
        if (candidate > 2 && (candidate & 1) == 0)
            candidate += 1;

        /* Find the next prime. */
        while (!is_prime_big(candidate))
            candidate += 2;

        bool ok;
        if (candidate == 2) {
            ok = true;          /* 2 always satisfies the condition */
        } else {
            int rem = (int)((candidate - 1) % 42);
            int d = gcd_i32(42, rem);
            seq_t exp = (candidate - 1) / (seq_t)d;
            seq_t val = mod_pow_big((seq_t)2, exp, candidate);
            ok = (val == (seq_t)1);
        }

        if (ok) {
            if (len == cap) {
                cap <<= 1;
                primes = mem_resize(seq_t, primes, cap);
            }
            primes[len++] = candidate;
        }

        last_checked = candidate;
        candidate = candidate + (candidate == 2 ? 1 : 2);
    }

    return primes[n - 1];
}
