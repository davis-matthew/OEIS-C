/*
 * A144591  Composites of the form smallest digit of n + prime(n).
 * https://oeis.org/A144591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A144591
 *  a(n) = smallest digit of k + prime(k) for the n-th k such that
 *  the sum is composite.
 *  The sequence lists the composite values in the order of increasing k.
 *
 *  Implementation details:
 *  - k is iterated from 1 upwards.
 *  - For each k we compute the smallest decimal digit (0‑9).
 *  - We obtain the k-th prime via get_prime(k).
 *  - The sum is tested for compositeness with is_prime_big.
 *  - Results are cached so any order of calls works efficiently.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
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

/* Helper: smallest decimal digit of n (n >= 0). */
static unsigned min_digit_idx(idx_t n)
{
    unsigned d = 9;
    if (n == 0) return 0;
    while (n) {
        unsigned r = (unsigned)(n % 10);
        if (r < d) {
            d = r;
            if (d == 0) break;
        }
        n /= 10;
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    /* ----- static cache of computed composite sums ----- */
    static seq_t *cache      = NULL;      /* array of composite sums      */
    static size_t cache_len = 0;          /* number of stored terms        */
    static size_t cache_cap = 0;          /* allocated slots                */
    static idx_t  k_next    = 1;          /* next k to process             */

    /* ---- grow cache until we have n terms ---- */
    while (cache_len < (size_t)n) {
        /* Compute smallest digit of k_next */
        unsigned md = min_digit_idx(k_next);

        /* Get the k_next-th prime (1‑based). */
        uint32_t pk = get_prime(k_next);

        /* Sum = smallest digit + prime(k). */
        seq_t sum = (seq_t)md + (seq_t)pk;

        /* If sum is composite, store it. */
        if (!is_prime_big(sum)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sum;
        }

        ++k_next;
    }

    return cache[(size_t)n - 1];
}
