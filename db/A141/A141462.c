/*
 * A141462  Transformed nonprime products of prime factors of the composites,
 *          the largest prime decremented by 2, the smallest by 1.
 * https://oeis.org/A141462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k*(pmax-2)*(pmin-1)/(pmin*pmax) where k is the n-th
 *            composite, * pmin, pmax are the smallest and largest prime
 *            factors of k; * the sequence lists this transformed product only
 *            when it is not prime (zero * is included)
 * Range:     u8 n <= 9, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-03)
 * Helpers:   factor_u64, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_seq.h"
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

seq_t a(idx_t n) {
    /* Static storage for computed terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static idx_t terms_cnt = 0;   /* number of output terms stored */
    static uint64_t next_k = 4;   /* next composite to examine */

    /* Ensure we have at least n terms computed */
    if (n > terms_cnt) {
        /* Grow array capacity if necessary */
        size_t new_cap = terms_cap ? terms_cap : 128;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;

        /* Helper buffers for factorization */
        static uint64_t primes[64];
        static uint64_t exps[64];
        size_t prime_cnt;

        while (terms_cnt < n) {
            /* Find next composite k */
            while (is_prime_seq((seq_t)next_k)) ++next_k;
            uint64_t k = next_k;
            ++next_k;

            /* Factor k */
            factor_u64(k, primes, exps, &prime_cnt);
            uint64_t pmin = UINT64_MAX, pmax = 0;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes[i];
                if (p < pmin) pmin = p;
                if (p > pmax) pmax = p;
            }

            /* Compute transformed product */
            seq_t num = (seq_t)k *
                        (seq_t)(pmax - 2) *
                        (seq_t)(pmin - 1);
            seq_t denom = (seq_t)pmin * (seq_t)pmax;
            seq_t val = denom ? num / denom : 0;

            /* Store only if nonprime (zero is included) */
            if (!is_prime_big(val)) {
                terms[terms_cnt++] = val;
            }
        }
    }

    return terms[n - 1];
}
