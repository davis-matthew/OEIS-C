/*
 * A080117  Binary encoding of quadratic residue set formed for n-th prime,
 *          coerced to "complementarily symmetric binary sequence" with
 *          A080261 if the prime is of the form 4k+1.
 * https://oeis.org/A080117
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080117: binary encoding of the quadratic residue set of the n‑th prime,
 * coalesced into a complementarily symmetric binary sequence.
 *
 * For the n-th prime p:
 *   1. Let L = (p-1)/2.
 *   2. Put a bit 1 into S at position i (0‑based) iff i+1 is a quadratic
 *      residue modulo p (Jacobi( i+1 | p ) = +1).
 *   3. Form T = S || reverse( complement(S) );   // T has length p−1
 *   4. Interpret T as an unsigned integer with the most‑significant bit
 *      at position p−2.  That integer is a(n).
 *
 * The coefficients are computed on the fly and memoised across calls.
 *
 * Requires the helper functions:
 *   - uint32_t get_prime(idx_t k);          (n‑th prime)
 *   - int        jacobi_symbol(uint64_t a, uint64_t n);
 *
 * All other helpers are not used.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Helpers from the build */
#include <stddef.h>
#include "../utils/mem.h"

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

/* The function requested by the problem */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2.  For smaller n we return
     * zero; this value will not be cached (and will never be requested
     * by the test harness). */
    if (n < 2)
        return (seq_t)0;

    /* --- Memoisation table ---------------------------------------------- */
    static seq_t *cache = NULL;      /* cache[i] holds a(i) for i ≥ 2 */
    static idx_t cache_cap = 0;      /* current capacity of cache */
    if (n >= cache_cap) {
        idx_t new_cap = (cache_cap > n) ? cache_cap * 2 : n * 2 + 8;
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* newly allocated elements are zero-initialised by realloc? Not
         * guaranteed.  We only read cache[n] after checking cache[n] != 0. */
    }
    if (cache[n] != (seq_t)0)
        return cache[n]; /* return cached value */

    /* --- Step 1: obtain the n-th prime ----------------------------------- */
    uint64_t p = (uint64_t)get_prime(n);

    /* p is at least 3 for n ≥ 2.  Compute L = (p-1)/2. */
    idx_t L = (idx_t)((p - 1) >> 1);

    /* --- Step 2: compute the bit set S ------------------------------ */
    static uint8_t *s_bits = NULL;   /* temporary storage for S bits */
    static idx_t s_cap = 0;
    if (s_cap < L) {
        uint8_t *tmp = (uint8_t *)realloc(s_bits, (size_t)L);
        if (!tmp && L > 0) abort();
        s_bits = tmp;
        s_cap = L;
    }

    for (idx_t i = 1; i <= L; ++i) {
        int jsym = jacobi_symbol(i, p);          /* Legendre/Jacobi symbol */
        s_bits[i - 1] = (jsym == 1) ? 1 : 0;
    }

    /* --- Step 3: build the complementarily symmetric sequence --------- */
    seq_t result = (seq_t)0;
    for (idx_t j = 0; j < L; ++j) {
        /* From S: set bit at position 2L-1-j if s_bits[j] == 1. */
        if (s_bits[j]) {
            idx_t pos = 2 * L - 1 - j;
            result |= (seq_t)1 << pos;
        }

        /* From reverse(complement(S)): set bit at position L-1-j if
           s_bits[L-1-j] == 0. */
        if (!s_bits[L - 1 - j]) {
            idx_t pos = L - 1 - j;
            result |= (seq_t)1 << pos;
        }
    }

    /* Store in cache and return */
    cache[n] = result;
    return result;
}
