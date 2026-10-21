/*
 * A095220  Least multiple of prime(n) beginning with the concatenation of
 *          first n primes.
 * https://oeis.org/A095220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   digits_of, get_prime, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095220: Least multiple of prime(n) beginning with the
 * concatenation of the first n primes.  */
#include <stdint.h>
#include <stdbool.h>

/* Utility functions that provide the required operations. */
#include "../utils/get_prime.h"
#include "../utils/digits_of.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

/* ------------------------------------------------------------------ */
/* Computes a(n) for n ≥ 1.  The returned value fits in seq_t when it
 * fits in the underlying type.  A simple O(n) construction of the
 * concatenated prefix is used, followed by a linear search for the
 * smallest suffix that makes the whole number a multiple of prime(n). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n < 1; return 0 safely. */
        return 0;
    }

    /* 1. Build the concatenation P_n of the first n primes. */
    seq_t pref = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i);
        int d = digits_of((idx_t)p);            /* decimal digits of p */
        seq_t powd = pow10(d);                  /* 10^d as seq_t */
        pref = seq_mul(pref, powd);             /* pref * 10^d */
        pref += (seq_t)p;                       /* add prime itself   */
    }

    /* 2. If the prefix itself is already a multiple of prime(n),
     * that is the answer. */
    uint32_t p_n = get_prime(n);
    uint32_t prefMod = (uint32_t)(pref % (seq_t)p_n);
    if (prefMod == 0) {
        return pref;
    }

    /* 3. Search for the smallest suffix length L such that
     *    pref * 10^L + r is a multiple of p_n with r < 10^L.
     *    We keep track of 10^L (as a seq_t) and 10^L mod p_n. */
    seq_t pow10_big = 1;                /* 10^L as big integer */
    uint32_t pow10mod = 1;              /* 10^L modulo p_n      */
    int maxL = digits_of((idx_t)p_n) + 5; /* safe upper bound      */

    for (int L = 0; L <= maxL; ++L) {
        /* t = (prefMod * 10^L) mod p_n */
        uint32_t t = (uint32_t)((prefMod * (seq_t)pow10mod) % (seq_t)p_n);

        /* r = (p_n - t) mod p_n  (smallest non‑negative addend) */
        uint32_t r_val = (p_n - t) % p_n;

        /* Check if r_val < 10^L (pow10_big). */
        if ((seq_t)r_val < pow10_big) {
            seq_t result = seq_mul(pref, pow10_big);
            result += (seq_t)r_val;
            return result;
        }

        /* Prepare for next L: multiply everything by 10. */
        pow10mod = (pow10mod * 10u) % p_n;
        pow10_big = seq_mul(pow10_big, 10u);
    }

    /* The loop always finds a solution; return the prefix as a
     * fallback to satisfy the compiler. */
    return pref;
}
