/*
 * A137314  Numbers k such that a type-7 Gaussian normal basis over GF(2^k)
 *          exists.
 * https://oeis.org/A137314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A137314
   Numbers k such that a type-7 Gaussian normal basis over GF(2^k) exists.
   Condition: p = 7k + 1 is prime and either
        - ord_2(p) = p - 1
        - or ord_2(p) = k   and  gcd(k,7) = 1
   (ord_2(p) is the multiplicative order of 2 modulo p). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t *terms = NULL;      /* cached sequence terms */
static size_t terms_cap = 0;      /* allocated capacity */
static size_t terms_len = 0;      /* number of terms currently stored */
static uint64_t last_k = 0;       /* last k that was tested */

/* compute multiplicative order of 2 modulo prime p (p > 2, prime) */
static uint64_t order_two_prime(uint64_t p) {
    uint64_t m = p - 1;                   /* p - 1 = 7k */
    uint64_t order = m;
    uint64_t temp = m;

    /* factorize m and reduce order using the standard algorithm */
    for (uint64_t q = 2; q * q <= temp; q += (q == 2 ? 1 : 2)) {
        if (temp % q == 0) {
            while (temp % q == 0) temp /= q;   /* remove multiplicity */
            /* try to divide out q from order as long as possible */
            while (order % q == 0) {
                seq_t val = mod_pow((seq_t)2, (idx_t)(order / q), (seq_t)p);
                if (val == (seq_t)1)
                    order /= q;
                else
                    break;
            }
        }
    }
    if (temp > 1) {                          /* remaining prime factor */
        uint64_t q = temp;
        while (order % q == 0) {
            seq_t val = mod_pow((seq_t)2, (idx_t)(order / q), (seq_t)p);
            if (val == (seq_t)1)
                order /= q;
            else
                break;
        }
    }
    return order;
}

/* append value to dynamic array terms */
static void append_term(seq_t val) {
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined, but safe fallback */

    /* compute terms up to index n if necessary */
    while (terms_len < (size_t)n) {
        uint64_t k = last_k + 1;             /* next candidate k */
        uint64_t p = 7 * k + 1;              /* corresponding p */

        if (p > UINT64_MAX) break;           /* overflow guard */

        if (is_prime(p)) {
            uint64_t ord = order_two_prime(p);
            if (ord == p - 1) {
                append_term((seq_t)k);
            } else if (ord == k && (k % 7 != 0)) {
                append_term((seq_t)k);
            }
        }
        last_k = k;
    }

    return terms[n - 1];
}
