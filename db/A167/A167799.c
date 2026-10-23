/*
 * A167799  Numbers with primitive root -3.
 * https://oeis.org/A167799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Numbers n such that "-3" is a primitive root modulo n.
 * The algorithm
 *   - generates all candidates by scanning integers upward,
 *   - checks the divisor 2 separately (only powers 2^0 or 2^1 are allowed),
 *   - tests whether the odd part of the number is a pure prime power p^k,
 *   - verifies that -3 is a primitive root modulo the prime p,
 *   - and finally checks the lifting condition for higher prime powers.
 * The sequence is memoised in a dynamic array so that calls with arbitrary
 * indices are fast.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mod_pow.h"

typedef uint64_t idx_t; /* supplied by the build */

static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

static void ensure_primes(uint32_t limit) {
    if (prime_cap > 0 && prime_cnt > 0 && prime_list[prime_cnt-1] >= limit)
        return;

    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        prime_cnt = 0;
    }
    /* grow capacity */
    while (prime_cap <= limit) {
        prime_cap <<= 1;
    }
    prime_list = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));

    /* start searching from the next integer after the last prime found */
    uint32_t start = (prime_cnt == 0) ? 2 : prime_list[prime_cnt-1] + 1;
    for (uint32_t x = start; x <= limit; ++x) {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt && (uint32_t)prime_list[i] * prime_list[i] <= x; ++i) {
            if (x % prime_list[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = x;
        }
    }
}

static bool is_prime_small(idx_t n) {
    if (n < 2) return false;
    ensure_primes((uint32_t)2 + 100); /* small bound may be extended later */
    for (size_t i = 0; i < prime_cnt && (uint32_t)prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

static bool factor_prime_power(idx_t m, idx_t *p_out, size_t *k_out) {
    if (m < 2 || (m & 1ULL) == 0) return false;
    double d = sqrt((double)m);
    uint32_t limit = (uint32_t)(d + 1.0);
    ensure_primes(limit);
    idx_t p = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t q = prime_list[i];
        if ((idx_t)q * q > m) break;
        if (m % q == 0) { p = q; break; }
    }
    if (!p) p = m;          /* m is prime */
    size_t k = 0;
    idx_t tmp = m;
    while (tmp % p == 0) { tmp /= p; ++k; }
    if (tmp != 1) return false;   /* not a pure power */
    *p_out = p;
    *k_out = k;
    return true;
}

static bool is_primitive_mod_p(idx_t p) {
    if (p <= 2 || p % 3 == 0) return false;
    double d = sqrt((double)(p - 1));
    uint32_t limit = (uint32_t)(d + 1.0);
    ensure_primes(limit);
    seq_t base = (seq_t)p - 3ULL;   /* -3 mod p */
    idx_t phi = p - 1;
    idx_t phi_copy = phi;
    for (size_t i = 0; i < prime_cnt && (uint32_t)prime_list[i] * prime_list[i] <= phi_copy; ++i) {
        uint32_t q = prime_list[i];
        if (phi_copy % q == 0) {
            seq_t val = mod_pow(base, (uint64_t)(phi / q), (seq_t)p);
            if (val == (seq_t)1) return false;
            while (phi_copy % q == 0) phi_copy /= q;
        }
    }
    if (phi_copy > 1) {
        uint32_t q = (uint32_t)phi_copy;
        seq_t val = mod_pow(base, (uint64_t)(phi / q), (seq_t)p);
        if (val == (seq_t)1) return false;
    }
    return true;
}

static bool higher_powers_ok(idx_t p) {
    seq_t mod = (seq_t)p * (seq_t)p;
    seq_t base = mod - (seq_t)3;
    seq_t val = mod_pow(base, (uint64_t)(p - 1), mod);
    return val != (seq_t)1;
}

/* sequence storage */
static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t next_check = 2;  /* next integer to test */

static void ensure_seq_cap(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_vals = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void generate_terms(idx_t n) {
    ensure_primes(100); /* seed small primes */
    while (seq_len < n) {
        idx_t val = next_check++;
        if (val == 2) {          /* special case */
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)2;
            continue;
        }
        if (val % 3 == 0) continue;          /* must be coprime to 3 */

        /* factor out powers of 2 */
        idx_t x = val;
        size_t two_cnt = 0;
        while ((x & 1ULL) == 0) {
            x >>= 1;
            ++two_cnt;
            if (two_cnt >= 2) break; /* only 2^0 or 2^1 allowed */
        }
        if (two_cnt >= 2) continue;
        if (x == 1) continue;         /* handled only for val==2 */

        /* odd part must be a pure prime power */
        idx_t p; size_t k;
        if (!factor_prime_power(x, &p, &k)) continue;
        if (p == 3) continue;
        if (!is_primitive_mod_p(p)) continue;
        if (k > 1 && !higher_powers_ok(p)) continue;

        /* accept the number */
        if (two_cnt == 1) {
            /* val is 2*x */
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)(2 * x);
        } else {
            /* odd number x itself */
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)x;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n=0 */
    generate_terms(n);
    return seq_vals[n - 1];
}
