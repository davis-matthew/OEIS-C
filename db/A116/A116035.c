/*
 * A116035  Numbers k such that k + phi(k) + sigma(k) is a prime.
 * https://oeis.org/A116035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 991 (2026-11-03)
 * Helpers:   is_prime_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k + phi(k) + sigma(k) is prime (OEIS A116035). */
/* The function iteratively finds k's that satisfy the condition, caches
   them, and returns the nth such k (1‑based).  The algorithm uses
   calc_phi, sigma_of, and is_prime_big from the utilities. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper prototypes */
#include <stdint.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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

/* external linkage as required */
seq_t a(idx_t n)
{
    /* Items are defined only for n >= 1.  For n < 1 simply return 0. */
    if (n < 1)
        return 0;

    /* Static cache of the found terms */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t next_k = 1;      /* next k to test */

    /* Keep generating until we have at least n terms */
    while (cache_len < n) {
        /* Compute the value k + phi(k) + sigma(k) */
        idx_t k_i = next_k;          /* integer k */
        seq_t k = (seq_t)k_i;        /* convert to seq_t for arithmetic */

        seq_t sum = k + totient_seq(k) + (seq_t)sigma_of(k_i);

        if (is_prime_big(sum)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
