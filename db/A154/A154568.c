/*
 * A154568  Composites of the form (sum of digits of k) + (sum of digits of
 *          prime(k)).
 * https://oeis.org/A154568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th composite integer of the form sum_of_digits(k) +
 *            sum_of_digits(prime(k)), k ≥ 1
 * Range:     u8 n <= 38, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <limits.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
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

static seq_t *cache = NULL;
static size_t  cache_len = 0, cache_cap = 0;
static idx_t   next_k = 1;
static bool    initialized = false;

/* Ensure the cache array can hold at least `cap` elements. */
static void ensure_cache_cap(size_t cap)
{
    if (cache_cap >= cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < cap) new_cap = cap;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Sum of decimal digits of an idx_t value. */
static seq_t digit_sum_idx(idx_t x)
{
    seq_t s = 0;
    while (x) {
        s += (seq_t)(x % 10);
        x /= 10;
    }
    return s;
}

/* Return true iff v is a composite integer (≥4 and not prime). */
static bool is_composite(seq_t v)
{
    if (v < 4) return false;          /* 0,1,2,3 are not composite */
    return !isPrimeGeneric(v);         /* isPrimeGeneric returns true if prime */
}

/* ---
   External function required by the OEIS build system. 
   Computes the n-th term of sequence A154568. 
   The function is memoised, so subsequent calls are fast. 
   --- */
seq_t a(idx_t n)
{
    /* Initialise static data structures if this is the first call. */
    if (!initialized) {
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        next_k = 1;
        initialized = true;
    }

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        /* Compute prime(k) and the required digit sums. */
        uint32_t prime_k = get_prime(next_k);
        seq_t    sum_k   = digit_sum_idx(next_k);
        seq_t    sum_p   = digit_sum((seq_t)prime_k);
        seq_t    total   = sum_k + sum_p;

        /* Store the composite sums. */
        if (is_composite(total)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = total;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
