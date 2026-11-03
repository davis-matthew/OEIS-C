/*
 * A231474  Primes whose base-3 representation is also the base-5
 *          representation of a prime.
 * https://oeis.org/A231474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 237, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9934 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that the base‑3 digits of p interpreted as a
 * base‑5 number are also prime.  The function builds the sequence
 * lazily and caches all results, so calls with arbitrary index order
 * are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
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
    if (n == 0) return 0;                 /* undefined for n < 1 */

    /* ---  static cache  ------------------------------------------ */
    static seq_t *cache = NULL;           /* array of found primes   */
    static size_t cache_len = 0;          /* number stored so far   */
    static idx_t checked = 1;             /* last integer inspected */

    /* ---  find further terms until we have n of them -------------- */
    while (cache_len < (size_t)n)
    {
        /* advance to next candidate */
        checked++;                        /* start from 2 onward */
        if (!is_prime(checked))
            continue;                    /* not a prime → skip */

        /* convert candidate to base‑3 digits (least significant first) */
        idx_t  t    = checked;
        idx_t  digs[128];
        size_t len  = 0;

        while (t > 0)
        {
            digs[len++] = t % 3;
            t           = t / 3;
        }
        if (len == 0) digs[len++] = 0;    /* handle the (impossible) case 0 */

        /* interpret those digits as a base‑5 number (most significant
           first) */
        seq_t q = 0;
        for (size_t i = 0; i < len; ++i)
            q = q * 5 + (seq_t)digs[len - 1 - i];

        /* both numbers must be prime */
        if (is_prime_big(q))
        {
            /* grow the cache if necessary */
            if (cache_len == 0)
                cache = mem_new(seq_t, 1);
            else
                cache = mem_resize(seq_t, cache, (cache_len + 1));

            cache[cache_len++] = checked;
        }
    }

    return cache[n - 1];
}
