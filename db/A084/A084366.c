/*
 * A084366  Numbers k such that the k-th prime number has an odd number of
 *          digits and becomes composite if the central digit is removed.
 * https://oeis.org/A084366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A084366: k such that the k-th prime has an odd number of digits and
 * becomes composite if the central digit is removed.
 * The function builds the sequence lazily and memoises results.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
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

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* 
 * Compute the nth term of the sequence (offset 1).
 * The implementation uses a lazy search: primes are generated one by one
 * with get_prime() and tested until the required number of terms has been
 * found.  Results are cached so subsequent calls are fast.
 */
seq_t a(idx_t n)
{
    /* static state for lazy computation and memoisation */
    static idx_t test_idx   = 0;          /* last prime index examined */
    static idx_t found_cnt  = 0;          /* how many terms have been found */
    static seq_t *cache    = NULL;        /* array of found k values */
    static size_t cap      = 0;           /* cache capacity */
    static size_t len      = 0;           /* current number of cached terms */

    /* ensure cache is large enough */
    if (len + 1 > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        cache = mem_resize(seq_t, cache, new_cap);
        cap = new_cap;
    }

    /* if the requested term is already cached, return it */
    if (n <= len)
        return cache[n - 1];

    /* search for more terms until we reach the nth one */
    while (found_cnt < n) {
        idx_t prime_idx = ++test_idx;          /* next prime index */
        seq_t prime = get_prime(prime_idx);    /* k-th prime */

        /* number of decimal digits of the prime */
        idx_t digits = num_digits(prime);
        if (digits & 1) {                      /* odd number of digits? */
            idx_t half = digits / 2;            /* position of central digit */

            /* compute 10^half and 10^(half+1) */
            seq_t pow10_half = 1;
            seq_t pow10_half_plus = 1;
            for (idx_t i = 0; i < half; ++i) {
                pow10_half *= 10;
            }
            pow10_half_plus = pow10_half * 10;  /* 10^(half+1) */

            /* high part (digits before central), low part (after) */
            seq_t high = prime / pow10_half_plus;
            seq_t low  = prime % pow10_half;
            seq_t removed = high * pow10_half + low;

            /* composite after removal? */
            if (removed > (seq_t)1 && !isPrimeGeneric(removed)) {
                /* store the prime index (k) */
                cache[found_cnt] = prime_idx;
                ++found_cnt;
                ++len;   /* cache size grows with each new term */
            }
        }
    }

    return cache[n - 1];
}
