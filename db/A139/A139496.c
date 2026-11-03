/*
 * A139496  Primes of the form x^2 + 13x*y + y^2 for x and y nonnegative.
 * https://oeis.org/A139496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x^2 + 13xy + y^2 for non‑negative x,y, sorted increasingly.
   a(n) = n‑th such prime. */

/* The routine keeps a static list of all primes found so far, growing the
   search limit geometrically until at least n primes are collected.  The
   list is kept sorted and deduplicated after each expansion.  This allows
   calls in arbitrary order without recomputing from scratch. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Compare two seq_t values for qsort. */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* Integer square root for idx_t (uint64_t).  Returns ⌊√n⌋. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid)        /* mid*mid > n without overflow */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* static tables reused between calls */
    static seq_t *primes = NULL;   /* sorted unique primes found */
    static size_t prime_cap = 0;
    static size_t prime_cnt = 0;
    static idx_t  prev_limit = 0;   /* highest value already processed */

    if (n == 0) {          /* offset is 1 – behaviour for 0 is irrelevant */
        return (seq_t)0;
    }

    /* Grow the prime list until we have at least n primes */
    while (prime_cnt < n) {
        /* Increase search limit geometrically */
        idx_t new_limit = (prev_limit == 0) ? 1000 : prev_limit * 2;
        if (new_limit <= prev_limit)          /* safety against overflow */
            new_limit = prev_limit + 1;

        idx_t max_y = isqrt_u64(new_limit);

        /* Generate all candidates <= new_limit and > prev_limit */
        for (idx_t y = 0; y <= max_y; ++y) {
            for (idx_t x = 0;; ++x) {
                unsigned __int128 val128 =
                    (unsigned __int128)x * x +
                    (unsigned __int128)13 * x * y +
                    (unsigned __int128)y * y;

                if (val128 > (unsigned __int128)new_limit)
                    break;
                if (val128 <= (unsigned __int128)prev_limit)
                    continue;

                seq_t val = (seq_t)val128;
                if (is_prime_big(val)) {
                    /* store prime if not already present */
                    if (prime_cnt == prime_cap) {
                        size_t new_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
                        seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                        if (!tmp) {
                            mem_free(primes);
                            return (seq_t)0;
                        }
                        primes = tmp;
                        prime_cap = new_cap;
                    }
                    primes[prime_cnt++] = val;
                }
            }
        }

        /* Sort and deduplicate the prime list */
        if (prime_cnt > 0) {
            mem_qsort(primes, prime_cnt, sizeof(seq_t), seq_compare);
            size_t uniq = 0;
            for (size_t i = 0; i < prime_cnt; ++i) {
                if (uniq == 0 || primes[i] != primes[uniq - 1]) {
                    primes[uniq++] = primes[i];
                }
            }
            prime_cnt = uniq;
        }

        prev_limit = new_limit;
    }

    return primes[n - 1];
}
