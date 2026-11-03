/*
 * A161958  a(n) = smallest composite number which is the sum of n consecutive
 *          composite numbers.
 * https://oeis.org/A161958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite number which is the sum of n *
 *            consecutive composite numbers. This implementation keeps a list
 *            * of composite numbers and their prefix sums, then scans for the
 *            * first composite sum of a given length. Results are cached * so
 *            that arbitrary calls to a(n) are efficient
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
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

/* Static storage for composite numbers and their prefix sums. */
static seq_t *comps = NULL;          /* array of composite numbers */
static seq_t *pref = NULL;           /* prefix sums: pref[0] = 0,
                                       * pref[i+1] = sum(comps[0..i]) */
static size_t comp_cap = 0;          /* current capacity of comps/pref pair */
static size_t comp_cnt = 0;          /* number of composites stored */
static seq_t next_candidate = 4;     /* next integer to test for compositeness */

/* Cache of computed a(n) values. */
static seq_t *a_cache = NULL;
static size_t cache_cap = 0;

/* Ensure the composite arrays have at least `min_cap` capacity. */
static void ensure_composite_capacity(size_t min_cap)
{
    if (comp_cap >= min_cap)
        return;
    size_t new_cap = comp_cap ? comp_cap * 2 : 64;
    if (new_cap < min_cap)
        new_cap = min_cap;
    comps = mem_resize(seq_t, comps, new_cap);
    pref  = mem_resize(seq_t, pref, (new_cap + 1));
    comp_cap = new_cap;
    /* Pref[0] must be 0. */
    if (pref != NULL)
        pref[0] = 0;
}

/* Generate composite numbers until we have at least `needed` composites. */
static void generate_composites(size_t needed)
{
    while (comp_cnt < needed) {
        /* Skip primes to find the next composite. */
        while (is_prime_big(next_candidate))
            next_candidate++;
        /* Store the composite. */
        ensure_composite_capacity(comp_cnt + 1);
        comps[comp_cnt] = next_candidate;
        pref[comp_cnt + 1] = pref[comp_cnt] + comps[comp_cnt];
        comp_cnt++;
        next_candidate++;
    }
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    /* Offset: a(1) is the first term.  We ignore a(0). */
    if (n == 0)
        return 0;

    size_t idx = (size_t)n;  /* 1‑based index for the cache */

    /* Resize the cache if necessary. */
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;
        if (a_cache == NULL)
            a_cache = mem_new(seq_t, new_cap);
        else
            a_cache = mem_resize(seq_t, a_cache, new_cap);
        /* Initialise new entries to 0 (sentinel). */
        if (cache_cap < new_cap)
            for (size_t i = cache_cap; i < new_cap; ++i)
                a_cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Return cached value if it exists. */
    if (a_cache[idx] != 0)
        return a_cache[idx];

    /* Find the smallest composite sum of `idx` consecutive composites. */
    size_t start = 0;
    while (true) {
        size_t need = start + idx;  /* number of composites required */
        generate_composites(need);
        seq_t sum = pref[need] - pref[start];
        if (!is_prime_big(sum)) {   /* composite sum found */
            a_cache[idx] = sum;
            return sum;
        }
        ++start;
    }
}
