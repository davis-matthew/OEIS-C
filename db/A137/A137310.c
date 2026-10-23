/*
 * A137310  Numbers k such that a type-4 Gaussian normal basis over GF(2^k)
 *          exists.
 * https://oeis.org/A137310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 4308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A137310: a(n) = the n‑th integer k such that 4k+1 is prime,
   4k+1 ≡ 5 (mod 8) and 2 is a primitive root modulo 4k+1.  The function
   generates terms on demand and keeps a static cache so that calls in
   any order are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"

static seq_t *terms = NULL;      /* cached sequence values */
static size_t terms_cap = 0;     /* capacity of terms array */
static size_t terms_cnt = 0;     /* number of cached values */
static uint64_t next_k = 1;      /* next k to test */

/* Test whether 2 is a primitive root modulo the prime p */
static bool isPrimitiveRoot2(uint64_t p)
{
    uint64_t phi = p - 1;       /* p is prime */
    uint64_t n = phi;
    uint64_t q;

    for (q = 2; q * q <= n; ++q) {
        if (n % q == 0) {
            /* divide out all copies of q */
            do { n /= q; } while (n % q == 0);
            if (powmod64(2, phi / q, p) == 1)
                return false;
        }
    }
    if (n > 1) {                /* remaining prime factor */
        if (powmod64(2, phi / n, p) == 1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (target == 0) return (seq_t)0; /* safety for offset 1 */

    /* Generate new terms until we have enough */
    while (terms_cnt < target) {
        for (uint64_t k = next_k; ; ++k) {
            uint64_t p = 4 * k + 1;
            if (is_prime(p) && (p & 7ULL) == 5ULL && isPrimitiveRoot2(p)) {
                /* resize cache if needed */
                if (terms_cnt + 1 > terms_cap) {
                    terms_cap = terms_cap ? terms_cap * 2 : 16;
                    terms = (seq_t *)realloc(terms,
                                             terms_cap * sizeof(seq_t));
                }
                terms[terms_cnt++] = (seq_t)k;
                next_k = k + 1;        /* start next search after k */
                break;
            }
        }
    }
    return terms[target - 1];
}
