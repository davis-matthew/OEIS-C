/*
 * A154484  a(n+1)-+a(n) = prime, a(1)=0, a(2)=2.
 * https://oeis.org/A154484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2792, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9837 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=2.
   For n>2, a(n) is the smallest integer > a(n-1) such that
   a(n)-a(n-1) and a(n)+a(n-1) are both prime.  The function memoizes
   results so that calls with arbitrary n are fast and order‑independent. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;    /* 1‑based cache: cache[0] = a(1)                */
static size_t cap    = 0;      /* allocated capacity (in terms)                */
static idx_t  len    = 0;      /* number of precomputed terms                  */

/* Ensure cache has room for at least 'needed' elements.  Resizes via realloc. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t newcap = cap ? cap : 8;
    while (newcap < needed) newcap <<= 1u;
    seq_t *tmp = mem_resize(seq_t, cache, newcap);
    if (!tmp) exit(1);                     /* abort on OOM                      */
    cache = tmp;
    cap   = newcap;
}

/* Initialise cache with the two known terms. */
static void init_cache(void)
{
    ensure_capacity(2);
    cache[0] = (seq_t)0;   /* a(1) */
    cache[1] = (seq_t)2;   /* a(2) */
    len = 2;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    if (len == 0) init_cache();          /* first call initializes base terms */

    /* Extend cache up to requested index, if necessary. */
    if (n > len)
    {
        for (idx_t i = len + 1; i <= n; ++i)
        {
            seq_t prev  = cache[i - 2];   /* a(i-1)                      */
            seq_t cand  = prev + (seq_t)1;/* start search at prev+1      */

            while (1)
            {
                seq_t diff = cand - prev;
                seq_t sum  = cand + prev;
                if (is_prime_big(diff) && is_prime_big(sum))
                    break;                /* found valid a(i)              */
                cand = cand + (seq_t)1;   /* try next candidate            */
            }
            ensure_capacity(i);           /* guarantee space for a(i)      */
            cache[i - 1] = cand;          /* store a(i) at index i-1       */
        }
        len = n;
    }

    return cache[n - 1];                  /* retrieve a(n)                  */
}
