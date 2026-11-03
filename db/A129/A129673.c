/*
 * A129673  Right truncatable primes in base 7 (written in decimal form).
 * https://oeis.org/A129673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: full, pass, 19 terms through n = 19 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n‑th term (1‑based) of OEIS sequence A129673
    – right truncatable primes in base 7 (decimal representation).
    The value is computed by exploring all base‑7 primes and storing them
    in increasing decimal order.  For n greater than the number of
    computable terms (depending on the size of seq_t) this function
    returns 0. */
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

/* Comparison function for qsort (works for any seq_t) */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Return the n‑th right truncatable prime in base 7 (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of sorted terms, lazily built on first call. */
    static seq_t *cache          = NULL;
    static size_t cache_len      = 0;
    static bool initialized      = false;

    /* Build the cache once. */
    if (!initialized) {
        /* Use a dynamic vector to accumulate primes. */
        size_t cap = 16;
        size_t sz  = 0;
        seq_t *vec = mem_new(seq_t, cap);
        if (!vec) return 0; /* allocation failure */

        /* Single‑digit base‑7 primes: 2, 3, 5. */
        vec[sz++] = (seq_t)2;
        vec[sz++] = (seq_t)3;
        vec[sz++] = (seq_t)5;

        /* Breadth‑first expansion: for each prime q, try all digits d=0..6. */
        for (size_t i = 0; i < sz; ++i) {
            seq_t q = vec[i];
            for (int d = 0; d <= 6; ++d) {
                seq_t cand = q * (seq_t)7 + (seq_t)d;
                /* Only test if cand fits in seq_t; if not, the cast will
                   wrap, so the primality test will fail – hence we ignore
                   over‑overflowing candidates. */
                if (is_prime_big(cand)) {
                    if (sz == cap) {
                        cap *= 2;
                        seq_t *tmp = mem_resize(seq_t, vec, cap);
                        if (!tmp) { mem_free(vec); return 0; }
                        vec = tmp;
                    }
                    vec[sz++] = cand;
                }
            }
        }

        /* Sort the collected primes in increasing order. */
        mem_qsort(vec, sz, sizeof(seq_t), cmp_seq_t);

        /* Allocate and copy into the cache. */
        cache = mem_new(seq_t, sz);
        if (!cache) { mem_free(vec); return 0; }
        for (size_t i = 0; i < sz; ++i) cache[i] = vec[i];
        mem_free(vec);
        cache_len = sz;
        initialized = true;
    }

    /* Return the requested term, or 0 if n is out of bounds. */
    if (n == 0) return 0;
    if (n <= (idx_t)cache_len) return cache[(size_t)(n - 1)];
    return 0;
}
