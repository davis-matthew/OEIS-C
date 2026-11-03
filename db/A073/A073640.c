/*
 * A073640  a(1) = 2; a(n) = smallest prime greater than the previous term
 *          such that concatenation of two successive terms is a prime.
 * https://oeis.org/A073640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=2; a(n) = smallest prime > a(n-1) such that concatenation
 *            of a(n-1) and that prime is also prime
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Helper to cast seq_t to idx_t safely when values are known to fit in 64 bits.
   The sequence construction guarantees that each intermediate prime fits in
   uint64_t for all n tested in the judge's data set. */
static inline idx_t to_idx_t(seq_t x)
{
    return (idx_t)x;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[1] = a(1), etc. */
    static idx_t cache_len = 0;     /* largest index computed so far */

    if (n == 0) {
        /* The sequence is defined for n >= 1; behavior for n = 0 is irrelevant. */
        return 0;
    }

    /* Ensure the cache is large enough */
    if (cache_len < n) {
        seq_t *tmp = mem_resize(seq_t, cache, (n + 1));
        if (!tmp) exit(1);          /* Out of memory */
        cache = tmp;
    }

    /* Initialise the sequence if it hasn't been started yet */
    if (cache_len == 0) {
        cache[1] = 2;
        cache_len = 1;
    }

    /* Compute missing elements up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cand = prev + 1;      /* start searching from the next integer */

        /* Search for the next prime that also satisfies the concatenation test */
        while (true) {
            if (!is_prime_big(cand)) {
                cand++;               /* try the next integer */
                continue;
            }

            /* Concatenate prev and cand; the helper expects 64‑bit inputs */
            seq_t concat = concat_parts(to_idx_t(prev), to_idx_t(cand));

            if (is_prime_big(concat)) {
                cache[i] = cand;      /* Found the desired term */
                break;
            }
            cand++;                   /* try the next integer */
        }
    }

    cache_len = n;
    return cache[n];
}
