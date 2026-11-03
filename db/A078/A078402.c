/*
 * A078402  Numbers k such that k^2 + 5 is prime.
 * https://oeis.org/A078402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078402: k such that k^2 + 5 is prime.
 * All terms are multiples of 6 (k = 0, 6, 12, …).
 * This implementation generates terms on demand by testing successive
 * multiples of 6 and storing the ones whose square plus five is prime.
 * It uses the utility function is_prime_big() for primality testing.
 */

#include <stdlib.h>          /* for malloc, realloc, abort */
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

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cached_len = 0;   /* number of terms stored in cache      */
static size_t cached_cap = 0;   /* allocated capacity of cache          */
static idx_t next_m = 0;        /* next integer m to test (k = 6*m)    */

/* Ensure the cache has at least `need` slots. */
static void
ensure_cache(size_t need)
{
    if (need <= cached_cap)
        return;
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (tmp == NULL)
        abort();                 /* cannot allocate more memory */
    cache = tmp;
    cached_cap = new_cap;
}

/* Generate terms until the cache contains at least `n` entries. */
static void
generate_up_to(idx_t n)
{
    if ((size_t)n <= cached_len)
        return;

    ensure_cache((size_t)n);

    while (cached_len < (size_t)n)
    {
        /* candidate k = 6 * next_m */
        seq_t k = (seq_t)(6 * next_m);
        seq_t candidate = k * k + (seq_t)5;   /* compute k^2 + 5 */

        /* Test primality of candidate. */
        if (is_prime_big(candidate))
            cache[cached_len++] = k;

        /* Always increment next_m to advance the search space. */
        ++next_m;
    }
}

/* Return a(n) according to the OEIS offset (a(1) is the first listed term). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* outside the defined offset */

    generate_up_to(n);
    return cache[(size_t)n - 1];
}
