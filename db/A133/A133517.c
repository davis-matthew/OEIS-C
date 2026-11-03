/*
 * A133517  Smallest k such that p(n)^3 - k is prime where p(n) is the n-th
 *          prime.
 * https://oeis.org/A133517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 95 terms through n = 955 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = smallest k such that p(n)^3 - k is prime
 * where p(n) is the nth prime (1‑indexed).  For n=1, k must be odd,
 * otherwise k must be even.  The function uses the provided
 * utilities get_prime, is_prime_small and is_prime_big.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/is_prime_u64.h"

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

static inline bool seq_is_prime(seq_t x)
{
    /* Select the appropriate primality routine depending on the
     * size of seq_t at compile time.  For 64‑bit (or smaller) types
     * use the fast trial version; otherwise fall back to the generic
     * arbitrary‑precision test. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return is_prime_seq(x);
    else
        return is_prime_big(x);
}

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑indexed) as a 32‑bit integer and cast it
     * to seq_t.  The prime table utilities guarantee that the
     * requested prime fits into uint32_t. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Compute the cube p^3.  Intermediate values are all of type
     * seq_t to avoid overflow in integral types smaller than 64 bits. */
    seq_t base = p * p * p;

    /* Special case for n = 1: base is even, so k must be odd.
     * For n > 1: base is odd, so k must be even. */
    seq_t k = (n == 1) ? (seq_t)1 : (seq_t)2;

    for (;;) {
        seq_t candidate = base - k;
        if (seq_is_prime(candidate))
            return k;
        k += (n == 1) ? (seq_t)2 : (seq_t)2;
    }
}
