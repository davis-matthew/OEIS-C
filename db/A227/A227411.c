/*
 * A227411  Palindromic prime numbers representing a date in "condensed
 *          European notation" DDMMYY.
 * https://oeis.org/A227411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   a(n) is the nth palindromic prime number of the form DDMMYY
 *            representing a valid European date in condensed decimal
 *            notation. The sequence is finite (14 terms)
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: full, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_palindrome_base, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_palindrome_base.h"
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

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Comparison function for qsort on seq_t values. */
static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;

    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Build the cache lazily on first use. */
static void build_cache(void)
{
    /* Days per month, February fixed to 28 days. */
    static const int days_in_month[13] = {
        0,  /* unused index 0 */
        31, /* Jan */
        28, /* Feb */
        31, /* Mar */
        30, /* Apr */
        31, /* May */
        30, /* Jun */
        31, /* Jul */
        31, /* Aug */
        30, /* Sep */
        31, /* Oct */
        30, /* Nov */
        31  /* Dec */
    };

    size_t cap = 16;  /* initial capacity for cache */
    cache = mem_new(seq_t, cap);
    if (!cache) abort();  /* fatal allocation failure */

    size_t len = 0;  /* number of elements currently stored */

    for (int y = 0; y <= 99; ++y) {
        for (int m = 1; m <= 12; ++m) {
            int days = days_in_month[m];
            for (int d = 1; d <= days; ++d) {
                /* Build decimal value DDMMYY. */
                uint64_t tmp = (uint64_t)d * 10000ULL
                            + (uint64_t)m * 100ULL
                            + (uint64_t)y;
                seq_t val = (seq_t)tmp;

                if (is_palindrome_base(val, 10) && is_prime_big(val)) {
                    if (len == cap) {
                        cap <<= 1;
                        seq_t *p = mem_resize(seq_t, cache, cap);
                        if (!p) { mem_free(cache); abort(); }
                        cache = p;
                    }
                    cache[len++] = val;
                }
            }
        }
    }

    /* Sort the collected values to provide them in increasing order. */
    mem_qsort(cache, len, sizeof(seq_t), cmp_seq);
    cache_len = len;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* undefined for n = 0 */

    if (!cache) build_cache();                   /* lazily initialize */

    if (n <= (idx_t)cache_len) return cache[n - 1];
    return (seq_t)0;                             /* beyond finite sequence */
}
