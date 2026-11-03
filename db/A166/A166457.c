/*
 * A166457  Numbers n such that n*100+1 is prime.
 * https://oeis.org/A166457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer m such that 100*m+1 is prime
 * Range:     u8 n <= 1, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8820 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1 (1*100+1 = 101 is prime) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* static cache for memoised terms */
static seq_t *cache = NULL;    /* array of computed a(k) */
static size_t cache_cap = 0;   /* current capacity of cache */
static size_t cache_len = 0;   /* number of terms stored in cache */
static idx_t  last_checked = 0;/* last integer m that was tested for 100*m+1 */

/* return the n-th term of sequence A166457 (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1; safe fallback */
        return (seq_t)0;

    /* fast return if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* generate terms until we have at least n entries */
    while (cache_len < n) {
        ++last_checked;                          /* next candidate m */
        seq_t candidate = (seq_t)last_checked * (seq_t)100 + (seq_t)1;
        if (is_prime_big(candidate)) {            /* 100*m+1 is prime? */
            if (cache_len == cache_cap)          /* grow cache if needed */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = last_checked;   /* store the m value */
        }
    }

    return cache[n - 1];
}
