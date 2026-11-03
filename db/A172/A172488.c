/*
 * A172488  Primes of the form 2^i * 3^j + 1 with i + j = 13.
 * https://oeis.org/A172488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: full, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes of the form 2^i * 3^j + 1 with i + j = 13,
   ordered increasingly. */

#include <stdlib.h>
#include <stdint.h>
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

/* Cached list of the 5 primes.  The cache is built on first call. */
static seq_t *cache = NULL;
static size_t cache_count = 0;

static void build_cache(void)
{
    const int SUM = 13;
    const int TOTAL = SUM + 1;          /* i ranges 0..13 inclusive */
    cache = mem_new(seq_t, TOTAL);
    cache_count = 0;

    for (int i = 0; i <= SUM; ++i) {
        int j = SUM - i;
        uint64_t val64 = (uint64_t)1 << i;          /* 2^i */
        for (int k = 0; k < j; ++k)
            val64 *= (uint64_t)3;                  /* * 3^j   */
        val64 += 1ULL;                              /* + 1     */
        seq_t val = (seq_t)val64;

        if (is_prime_big(val))
            cache[cache_count++] = val;
    }

    /* Sort ascending.  cache_count is at most 5.  */
    for (size_t p = 0; p + 1 < cache_count; ++p)
        for (size_t q = p + 1; q < cache_count; ++q)
            if (cache[p] > cache[q]) {
                seq_t tmp = cache[p];
                cache[p] = cache[q];
                cache[q] = tmp;
            }
}

seq_t a(idx_t n)
{
    if (n == 0 || n < 1)
        return 0;               /* behaviour below offset is irrelevant */

    if (cache == NULL)
        build_cache();

    if (n <= cache_count)
        return cache[n - 1];
    return 0;                   /* sequence is finite */
}
