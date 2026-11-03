/*
 * A130474  Prime numbers p of the form p=x^2+y^3 such that q =A130475(n)=
 *          abs(x^2-y^3) is prime and smaller than p.
 * https://oeis.org/A130474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A130474: primes p = x² + y³ such that q = |x²−y³| is prime
   and q < p.  The function enumerates all candidates up to a dynamic
   limit and caches them.  It works for arbitrary unsigned seq_t
   (uint8_t … __int128) and idx_t = uint64_t. */

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

static int compare_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static seq_t *generate_terms(uint64_t limit, size_t *out_len)
{
    /* limit is a 64‑bit bound; reduce it to the maximum representable
       value of seq_t. */
    uint64_t max_seq_val = (uint64_t)(seq_t)-1;
    if (limit > max_seq_val) limit = max_seq_val;

    size_t cap = 64;
    seq_t *terms = mem_new(seq_t, cap);
    if (!terms) abort();
    size_t len = 0;

    uint64_t y = 1;
    for (;;) {
        uint64_t y3 = y * y * y;
        if (y3 > limit) break;
        uint64_t x = 1;
        for (;;) {
            uint64_t x2 = x * x;
            uint64_t p64 = x2 + y3;
            if (p64 > limit) break;
            uint64_t q64 = (x2 >= y3) ? (x2 - y3) : (y3 - x2);
            if (q64 < 2) { ++x; continue; }

            seq_t p = (seq_t)p64;
            seq_t q = (seq_t)q64;

            if (is_prime_big(p) && is_prime_big(q) && q < p) {
                if (len + 1 > cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                    if (!tmp) abort();
                    terms = tmp;
                    cap = new_cap;
                }
                terms[len++] = p;
            }
            ++x;
        }
        ++y;
    }

    if (len > 1)
        mem_qsort(terms, len, sizeof(seq_t), compare_seq_t);

    seq_t *tmp = mem_resize(seq_t, terms, len);
    if (!tmp && len > 0) abort();
    terms = tmp;

    *out_len = len;
    return terms;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return 0; /* undefined for n = 0 */

    if (cache_len >= n)
        return cache[n - 1];

    /* initial estimate – grows as needed */
    uint64_t limit = (uint64_t)n * 200ULL;
    if (limit < 1000ULL) limit = 1000ULL;

    while (1) {
        size_t len;
        seq_t *terms = generate_terms(limit, &len);

        if (cache) mem_free(cache);
        cache = terms;
        cache_len = len;

        if (cache_len >= n)
            break;

        mem_free(cache);          /* discard produced terms */
        cache = NULL;
        cache_len = 0;
        limit *= 2ULL;        /* increase search range */
    }

    return cache[n - 1];
}
