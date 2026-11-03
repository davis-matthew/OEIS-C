/*
 * A020994  Primes that are both left-truncatable and right-truncatable.
 * https://oeis.org/A020994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   this code searches for primes that remain prime when any number
 *            of leading or trailing digits are removed. For a prime p all
 *            truncations from the left and from the right are tested. The
 *            function builds the complete list once and then returns the n‑th
 *            element (1‑based). Beyond the known 15 terms i...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: full, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/num_digits_idx.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t *two_sided_cache = NULL;
static size_t two_sided_count = 0;
static bool cache_built = false;

/* Test whether a prime p is truncatable from both sides. */
static bool is_two_sided_prime(uint64_t p)
{
    seq_t sp = (seq_t)p;
    int len = num_digits_idx(p);          /* number of decimal digits of p */
    for (int i = 1; i < len; ++i) {
        seq_t pow = pow10((idx_t)i);      /* 10^i */
        seq_t suffix = sp % pow;          /* last i digits */
        seq_t prefix = sp / pow;          /* first len-i digits */
        if (!is_prime_big(suffix) || !is_prime_big(prefix))
            return false;
    }
    return true;
}

/* Build the cache of all two‑sided truncatable primes. */
static void build_cache(void)
{
    if (cache_built) return;
    const uint64_t LIMIT = 1000000ULL;          /* enough to find all 15 terms */
    uint64_t *prime_array = NULL;
    size_t prime_cnt = 0;

    build_primes_upto(LIMIT, &prime_array, &prime_cnt);

    /* Allocate initial space for the results. */
    size_t cap = 16;
    two_sided_cache = mem_new(seq_t, cap);
    two_sided_count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_array[i];
        if (is_two_sided_prime(p)) {
            if (two_sided_count == cap) {
                cap *= 2;
                two_sided_cache = mem_resize(seq_t, two_sided_cache, cap);
            }
            two_sided_cache[two_sided_count++] = (seq_t)p;
        }
    }
    free(prime_array);
    cache_built = true;
}

/* External function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_cache();
    idx_t idx = n - 1;
    if ((size_t)idx >= two_sided_count) return 0;
    return two_sided_cache[idx];
}
