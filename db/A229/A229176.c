/*
 * A229176  Primes p with nonzero digits such that p + product_of_digits and p
 *          - product_of_digits are both prime.
 * https://oeis.org/A229176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th prime p with all non‑zero digits such that both
 *            p + prod(p) and p – prod(p) are prime (search)
 * Range:     u8 n <= 3, u16 n <= 357, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 967 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_product.h"
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

static seq_t *cache     = NULL;   /* stored terms                      */
static size_t cache_cap = 0;      /* capacity of the cache             */
static size_t cache_len = 0;      /* number of terms stored            */
static idx_t  next_candidate = 3; /* next odd number to test for prime */

static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache contains at least the first n terms. */
    while (cache_len < n)
    {
        /* Find the next prime candidate. */
        idx_t cand = next_candidate;
        while (!is_prime_big((seq_t)cand))
        {
            /* cand is odd (except possibly 3 at start), so we step by 2. */
            cand += 2;
        }

        /* Update pointer to the following odd candidate. */
        next_candidate = cand + 2;

        /* Must have all non‑zero digits: product_of_digits must be non‑zero. */
        seq_t prod = digit_product((seq_t)cand);
        if (!prod) continue;           /* skip if any digit is zero          */

        /* Both p ± prod must be prime. */
        seq_t p    = (seq_t)cand;
        seq_t sum  = p + prod;
        seq_t diff = p > prod ? p - prod : 0; /* diff > 0 for valid primes */

        if (!is_prime_big(sum) || !is_prime_big(diff))
            continue;                  /* not a member of the sequence       */

        /* Term qualifies; store it. */
        grow_cache(cache_len + 1);
        cache[cache_len++] = p;
    }

    return cache[n - 1];
}
