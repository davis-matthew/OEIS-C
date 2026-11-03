/*
 * A061365  Numbers k such that either k or its reversal (but not both) is
 *          prime.
 * https://oeis.org/A061365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 991 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A061365: Numbers k such that exactly one of k or rev(k) is prime.
    The routine is memoised; each evaluation may finish in O(1) amortised
    time after the first call.                                     */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
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

static seq_t *cache    = NULL;   /* buffer of found terms                */
static idx_t  cache_sz = 0;      /* number of stored terms              */
static idx_t  cache_cap = 0;     /* current capacity of cache           */
static seq_t  next_candidate = 1;/* next integer to examine             */

/* Ensure cache has room for at least 'need' elements.              */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) { abort(); } /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------
 *  Return the nth term of A061365 (offset = 1).  The function works
 *  correctly for any n >= 1, in any call order.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive guard – offset is 1 */

    while (cache_sz < n) {
        bool pk = is_prime_big(next_candidate);
        seq_t rev = reverse_digits(next_candidate);
        bool pr = is_prime_big(rev);
        if (pk != pr) {              /* exactly one of them is prime */
            grow_cache(cache_sz + 1);
            cache[cache_sz++] = next_candidate;
        }
        ++next_candidate;            /* proceed to the next integer   */
    }
    return cache[n - 1];
}
