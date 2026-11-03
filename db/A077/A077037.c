/*
 * A077037  Largest prime < n^3.
 * https://oeis.org/A077037
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime < n^3 (OEIS A077037)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 823 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

/* Static cache for previously computed values */
static seq_t *prime_cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    seq_t *tmp = mem_resize(seq_t, prime_cache, needed);
    if (!tmp)
        exit(1);
    prime_cache = tmp;

    /* Zero the newly added portion */
    if (cache_cap < needed)
        mem_set((prime_cache + cache_cap), 0, (needed - cache_cap) * sizeof(seq_t));

    cache_cap = needed;
}

/* Return the largest prime < n^3 */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for n < 2, harmless */

    ensure_cache(n);
    if (prime_cache[n] != 0)
        return prime_cache[n];

    /* Compute n^3 in seq_t arithmetic */
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;    /* n^3 */

    seq_t candidate = cube - 1;   /* start just below n^3 */

    /* Fast descent: skip even numbers (except 2) */
    if (candidate >= 3 && (candidate & 1) == 0)
        candidate--;              /* make it odd */

    while (candidate >= 2) {
        if (is_prime_big(candidate)) {
            prime_cache[n] = candidate;
            return candidate;
        }
        if (candidate == 3) {      /* next odd would be 1 */
            candidate = 2;
            continue;
        }
        candidate -= 2;            /* skip the even number below */
    }

    /* Fallback (should never happen) */
    prime_cache[n] = 2;
    return 2;
}
