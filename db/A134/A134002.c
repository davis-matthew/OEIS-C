/*
 * A134002  Positive integers n such that n(n+5)=a(a+5)+b(b+5) is solvable in
 *          positive integers.
 * https://oeis.org/A134002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134002
   a(n) = the n-th positive integer k such that k^2 + (k+5)^2 is composite.
   The function scans successive positive integers, tests the composite
   condition using is_prime_big, and caches the found terms for fast
   subsequent lookups. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t sz   = 0;       /* current number of stored terms */
    static size_t cap  = 0;       /* current capacity of `cache` */
    static idx_t next  = 1;       /* next integer to test */

    /* Grow cache until it holds at least n terms. */
    while (sz < (size_t)n) {
        seq_t k = (seq_t)next;                      /* candidate value */
        seq_t kk = k * k;                           /* k^2 */
        seq_t k5 = (k + 5) * (k + 5);               /* (k+5)^2 */
        seq_t sum = kk + k5;                        /* k^2 + (k+5)^2 */

        if (!is_prime_big(sum)) {                   /* composite -> term */
            if (sz == cap) {                        /* increase capacity */
                size_t newcap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = alloc_seq_array(newcap);
                if (!tmp) abort();
                for (size_t i = 0; i < sz; ++i)
                    tmp[i] = cache[i];
                if (cache)
                    free_seq_array(cache);
                cache = tmp;
                cap = newcap;
            }
            cache[sz++] = k;
        }
        ++next;
    }

    return cache[n - 1];
}
