/*
 * A142602  Primes congruent to 2 mod 55.
 * https://oeis.org/A142602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 2 (mod 55)
 * Range:     u8 n <= 2, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 989 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;      /* array of computed primes */
    static size_t cap   = 0;         /* allocated capacity */
    static size_t cnt   = 0;         /* number of terms stored */
    static idx_t  cand  = 2;         /* next candidate to test */

    if (n == 0) return 0;            /* offset is 1; n==0 is undefined */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) return 0;        /* allocation failed – return a sentinel */
        cache[0] = (seq_t)2;         /* first term 2 */
        cnt   = 1;
        cand  = 57;                  /* start of the arithmetic progression 55*k+2, k≥1 */
    }

    /* generate terms until we reach the requested index */
    while (cnt < n) {
        /* grow the cache if necessary */
        if (cnt >= cap) {
            size_t newcap = cap << 1;
            seq_t *tmp = mem_resize(seq_t, cache, newcap);
            if (!tmp) return 0;      /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        /* test the current candidate */
        if (is_prime_big((seq_t)cand))
            cache[cnt++] = (seq_t)cand;

        /* next number congruent to 2 modulo 55 */
        cand += 55;
    }

    return cache[n-1];
}
