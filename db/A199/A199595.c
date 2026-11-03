/*
 * A199595  Numbers n such that 3n+6, 3n+7 and 3n+8 are all composite.
 * https://oeis.org/A199595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th integer such that 3(k+5)+7 and 3(k+5)+8 are composite.
   Equivalently, let b(n) be the n-th integer for which 3b+7 and 3b+8 are
   composite; then a(n) = b(n) - 5.  This generates the OEIS sequence
   A199595 with offset 1. */
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
    /* Static cache for the generated terms (b(n) values) */
    static seq_t *cache     = NULL;   /* array of b(n) values */
    static size_t  cap      = 0;      /* current capacity of cache      */
    static size_t  sz       = 0;      /* number of terms stored         */
    static idx_t   next     = 1;      /* next integer to test          */

    if (n == 0) return 0; /* undefined for n<1 */

    /* Grow the cache if necessary */
    if (n > cap) {
        size_t newcap = cap ? cap << 1 : 64;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = newcap;
    }

    /* Generate terms until size reaches n */
    while (sz < n) {
        seq_t candidate = next;                                 /* k to test   */
        seq_t m1 = candidate * (seq_t)3 + (seq_t)7;              /* 3k+7       */
        seq_t m2 = candidate * (seq_t)3 + (seq_t)8;              /* 3k+8       */

        if (!is_prime_big(m1) && !is_prime_big(m2))
            cache[sz++] = candidate;                           /* store b(n) */

        ++next;
    }

    /* Return a(n) = b(n) - 5 */
    return cache[n - 1] - (seq_t)5;
}
