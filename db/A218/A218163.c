/*
 * A218163  a(n) is the smallest positive integer k such that k^32 + 1 == 0
 *          mod p, where p is the n-th prime of the form 1 + 64*b (see
 *          A142925).
 * https://oeis.org/A218163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A218163:
 * a(n) = the smallest positive integer k such that k^32 + 1 ≡ 0 (mod p_n),
 * where p_n is the nth prime of the form 1 + 64*b (see A142925). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *prime_form64 = NULL;      /* primes of the form 1+64*b */
static size_t prime_form_cap = 0;       /* allocated capacity */
static size_t prime_form_cnt = 0;       /* number of primes stored */

/* extend the prime_form64 array to contain at least needed primes */
static void ensure_prime_form64(size_t needed)
{
    if (prime_form_cnt >= needed) return;

    /* grow capacity if necessary */
    if (prime_form_cap < needed) {
        size_t new_cap = prime_form_cap ? prime_form_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_arr = (seq_t*)realloc(prime_form64, new_cap * sizeof(seq_t));
        if (!new_arr) return; /* allocation failure - let the caller fail */
        prime_form64 = new_arr;
        prime_form_cap = new_cap;
    }

    static idx_t next_b = 1;          /* next multiplier for 64*b+1 */
    while (prime_form_cnt < needed) {
        idx_t candidate = 1 + 64 * next_b++;
        if (is_prime((idx_t)candidate)) {
            prime_form64[prime_form_cnt++] = (seq_t)candidate;
        }
    }
}

/* Returns the array of distinct prime factors of m (as uint64_t) */
static void factor_distinct_uint64(uint64_t m, uint64_t *fac, size_t *cnt)
{
    *cnt = 0;
    while (m % 2 == 0) {
        if (*cnt == 0 || fac[*cnt-1] != 2) fac[(*cnt)++] = 2;
        m /= 2;
    }
    for (uint64_t f = 3; f * f <= m; f += 2) {
        if (m % f == 0) {
            fac[(*cnt)++] = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) fac[(*cnt)++] = m;
}

/* Finds a primitive root modulo p (p is prime), returns it as uint64_t */
static uint64_t primitive_root_mod_p(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t factors[64];
    size_t faccnt;
    factor_distinct_uint64(phi, factors, &faccnt);

    for (uint64_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < faccnt; ++i) {
            if (mod_pow((seq_t)g, phi / factors[i], (seq_t)p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return 0; /* should never happen */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, so a(0) undefined */
    size_t idx = (size_t)n;
    ensure_prime_form64(idx);
    if (idx == 0) return 0;        /* safety */

    seq_t p = prime_form64[idx - 1];   /* nth prime of form 1+64*b */
    seq_t p_minus1 = p - 1;

    /* find a primitive root g modulo p */
    uint64_t g = primitive_root_mod_p((uint64_t)p);
    if (g == 0) return 0;          /* safety, shouldn't occur */

    /* base exponent for solutions: (p-1)/64 */
    seq_t base = p_minus1 / 64;
    seq_t step = p_minus1 / 32;    /* increment for each of the 32 solutions */

    seq_t min_k = 0;
    bool first = true;
    for (uint32_t t = 0; t < 32; ++t) {
        seq_t exp = base + step * t;
        seq_t k = mod_pow((seq_t)g, (uint64_t)exp, p);
        if (first || k < min_k) {
            min_k = k;
            first = false;
        }
    }
    return min_k;
}
