/*
 * A181429  a(n)= the smallest number such that a(n)^2+1=p*A002144(n), p
 *          prime.
 * https://oeis.org/A181429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A181429: a(n) is the least integer a such that a²+1 = p·A002144(n)
   where p is a prime.  A002144(n) is the n‑th prime congruent to 1 (mod 4). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/build_primes.h"
#include "../utils/powmod.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

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

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Compute √(-1) mod a prime p.  p is assumed odd and p ≡ 1 (mod 4). */
static uint64_t sqrt_mod_minus_one(uint64_t p)
{
    /* Special case p ≡ 3 (mod 4) – not needed for A002144 primes but kept for safety. */
    if ((p & 3U) == 3U) {
        uint64_t exp = (p + 1) >> 2;              /* (p+1)/4 */
        seq_t r = powmod((seq_t)(p - 1), exp, (seq_t)p);
        return (uint64_t)r;
    }

    /* Tonelli–Shanks algorithm.  Write p‑1 = q·2^s with q odd. */
    uint64_t q = p - 1;
    uint32_t s = 0;
    while ((q & 1U) == 0U) { q >>= 1; ++s; }

    /* Find a quadratic non‑residue z. */
    uint64_t z = 2;
    while (jacobi_symbol(z, p) != -1) ++z;

    seq_t c = powmod((seq_t)z, q, (seq_t)p);
    seq_t t = powmod((seq_t)(p - 1), q, (seq_t)p);
    seq_t r = powmod((seq_t)(p - 1), (q + 1) >> 1, (seq_t)p); /* exp = (q+1)/2 */
    uint32_t m = s;

    while ((uint64_t)t != 1U) {
        /* Find least i (0 < i < m) with t^(2^i) ≡ 1 (mod p). */
        uint32_t i = 1;
        seq_t tt = (t * t) % (seq_t)p;
        while ((uint64_t)tt != 1U) { tt = (tt * tt) % (seq_t)p; ++i; }

        uint64_t exp = 1ULL << (m - i - 1);
        seq_t b = powmod(c, exp, (seq_t)p);

        r = (r * b) % (seq_t)p;
        t = (t * b * b) % (seq_t)p;
        c = (b * b) % (seq_t)p;
        m = i;
    }
    return (uint64_t)r;
}

/* Cache of primes ≡ 1 (mod 4). */
static uint64_t *prime_mod4   = NULL;
static size_t   prime_mod4_cnt = 0;

/* Make sure we have at least n primes 1 mod 4 cached. */
static void ensure_prime_mod4(idx_t n)
{
    if (prime_mod4_cnt >= (size_t)n) return;

    /* Estimate an upper bound and enlarge a little. */
    idx_t limit = _prime_limit_for_idx(2 * n) + 1000;
    idx_t *all = NULL;
    size_t cnt = 0;
    build_primes(limit, &all, &cnt);

    /* Keep every prime that is 1 mod 4. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = all[i];
        if ((p & 3U) == 1U) {
            if (prime_mod4_cnt % 16 == 0) {
                prime_mod4 = (uint64_t*)realloc(prime_mod4,
                    (prime_mod4_cnt + 16) * sizeof(uint64_t));
                if (!prime_mod4) abort();
            }
            prime_mod4[prime_mod4_cnt++] = p;
        }
    }
    free(all);
}

/* Does a satisfy the definition for the given q? */
static bool candidate_ok(seq_t a, seq_t q)
{
    seq_t prod = a * a + 1;
    if (prod % q != 0) return false;
    seq_t quot = prod / q;
    if (quot <= 1) return false;
    return is_prime_big(quot);
}

/* ------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    ensure_prime_mod4(n);

    uint64_t q = prime_mod4[n - 1];          /* n‑th prime ≡ 1 (mod 4) */
    seq_t   q_seq = (seq_t)q;

    uint64_t a0     = sqrt_mod_minus_one(q);
    uint64_t root1  = a0;
    uint64_t root2  = q - a0;
    if (root1 > root2) { uint64_t t = root1; root1 = root2; root2 = t; }

    uint64_t k = 0;
    while (true) {
        seq_t cand1 = (seq_t)root1 + (seq_t)k * (seq_t)q;
        if (candidate_ok(cand1, q_seq)) return cand1;

        seq_t cand2 = (seq_t)root2 + (seq_t)k * (seq_t)q;
        if (cand2 != cand1 && candidate_ok(cand2, q_seq)) return cand2;

        ++k;
    }
}
