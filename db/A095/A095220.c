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
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095220: Least multiple of prime(n) beginning with the
 * concatenation of the first n primes.  */
#include <stdint.h>
#include <stdbool.h>

/* Utility functions that provide the required operations. */
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/num_digits_idx.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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
        int d = num_digits_idx((idx_t)p);            /* decimal digits of p */
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
    int maxL = num_digits_idx((idx_t)p_n) + 5; /* safe upper bound      */

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
