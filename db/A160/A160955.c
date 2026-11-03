/*
 * A160955  a(n+1) is the largest prime formed by appending a single digit to
 *          a(n); a(1)=18.
 * https://oeis.org/A160955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n+1) is the largest prime which can be formed by appending a
 *            single decimal digit to a(n). Initial value: a(1)=18. If no such
 *            prime exists for the requested n, a(n) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: full, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Ensure we can use is_prime_big from the utils library */

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

/* Cache to store computed terms. */
static seq_t  *cache = NULL;   /* array of terms */
static size_t  cache_cap = 0;  /* current capacity of the array */
static size_t  cache_len = 0;  /* number of terms already computed */

/* Append a new term to the cache, reallocating if necessary. */
static inline void
append_cache(seq_t value)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
        if (!new_ptr) exit(EXIT_FAILURE);   /* allocation failure */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
}

/* Main sequence function as requested. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Ensure the first element is in the cache. */
    if (cache_len == 0)
        append_cache((seq_t)18);   /* a(1) = 18 */

    /* Generate terms until we reach the requested index. */
    while ((size_t)n > cache_len) {
        seq_t cur = cache[cache_len - 1];
        seq_t next = 0;
        bool found = false;

        /* Try digits from 9 down to 0 and stop at the first prime found. */
        for (int d = 9; d >= 0; --d) {
            seq_t cand = cur * ((seq_t)10) + (seq_t)d;
            if (is_prime_big(cand)) {
                next = cand;
                found = true;
                break;
            }
        }

        append_cache(next);   /* store either the prime found or 0 */

        /* If no prime was found, further terms will all be 0. */
        if (!found)
            break;
    }

    if (n <= (idx_t)cache_len)
        return cache[n - 1];
    else
        return (seq_t)0;   /* undefined for this n (no prime available) */
}
