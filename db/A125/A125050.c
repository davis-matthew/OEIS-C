/*
 * A125050  a(1) = 1. If a(n) is composite, a(n+1) = 2*a(n)+1; otherwise,
 *          a(n+1) = 2*a(n).
 * https://oeis.org/A125050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = 2*a(n)+1 if a(n) is composite, otherwise
 *            a(n+1) = 2*a(n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* static cache for memoisation */
    static seq_t *cache   = NULL;          /* array of already computed terms */
    static size_t cap     = 0;             /* allocated capacity */
    static idx_t  len     = 0;             /* number of terms stored */

    /* grow cache if needed */
    if ((size_t)len < n) {
        if (cap < (size_t)n) {             /* increase capacity */
            size_t newcap = cap ? cap : 8;
            while (newcap < (size_t)n) newcap <<= 1;
            seq_t *tmp = seq_alloc_array(newcap);
            for (size_t i = 0; i < (size_t)len; ++i) tmp[i] = cache[i];
            if (cache) seq_free_array(cache);
            cache = tmp;
            cap   = newcap;
        }

        /* initialize first term if cache empty */
        if (len == 0) {
            cache[0] = (seq_t)1;
            len = 1;
        }

        /* generate missing terms up to n */
        for (idx_t i = len; i < n; ++i) {
            seq_t prev = cache[i - 1];
            /* a(n) is composite iff prev > 1 && not prime */
            bool composite = (prev > (seq_t)1 && !is_prime_big(prev));
            cache[i] = prev * (seq_t)2 + (composite ? (seq_t)1 : (seq_t)0);
        }
        len = n;
    }

    return cache[n - 1];
}
