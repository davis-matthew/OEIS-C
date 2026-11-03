/*
 * A072185  Primes p = x1x2x3...xn in base 10 such that x1^p1 + x2^p2 + x3^p3
 *          + ... + xn^pn is a prime, where pn is the n-th prime.
 * https://oeis.org/A072185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   ipow, is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A072185: primes p = x1x2…xn such that
   ∑i=1..n x_i^{p_i} is prime, where p_i is the i‑th prime.
   a(n) is returned for n ≥ 1. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow.h"
#include "../utils/is_prime.h"
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cache_len = 0;
    static size_t   cache_cap = 0;
    static idx_t    candidate = 2;

    if (n == 0) return 0;

    while (cache_len < (size_t)n) {
        /* Find the next prime candidate. */
        while (!is_prime(candidate))
            ++candidate;

        /* Extract the decimal digits of the candidate (most‑significant first). */
        int d[64];
        int dlen = 0;
        idx_t tmp = candidate;
        if (tmp == 0) {
            d[dlen++] = 0;
        } else {
            while (tmp > 0) {
                d[dlen++] = (int)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Compute the sum Σ x_i^{p_i}. */
        seq_t sum = 0;
        for (int i = 0; i < dlen; ++i) {
            int digit = d[dlen - 1 - i];                     /* most‑significant first */
            seq_t term   = ipow((seq_t)digit,
                                    (seq_t)get_prime((idx_t)(i + 1)));
            sum += term;
        }

        /* If the sum is prime, store the candidate. */
        if (is_prime_big(sum)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 4;
                seq_t *tmp_arr = mem_resize(seq_t, cache, new_cap);
                if (!tmp_arr) abort();
                cache = tmp_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }

        ++candidate;  /* advance to the next integer */
    }

    return cache[n - 1];
}
