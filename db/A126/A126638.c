/*
 * A126638  a(n) is larger than a(n-1), not prime and coprime to a(n-1) and
 *          a(n-2).
 * https://oeis.org/A126638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 974 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) is the smallest composite > a(n‑1) that is coprime
   to a(n‑1) and a(n‑2).  Initial values:
   a(1) = 4, a(2) = 9, a(3) = 25. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
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
    /* cache of already‑computed terms, 0‑based: cache[0] = a(1) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;       /* allocated elements */
    static size_t used  = 0;       /* last filled index+1 (so used==0 means none) */

    if (n == 0) return 0; /* offset is 1, this value never used */

    /* Return cached value if already available */
    if (n <= used) {
        return cache[n - 1];
    }

    /* Grow cache if necessary */
    if (cap < n) {
        size_t newcap = cap ? cap << 1 : 4;
        if (newcap < n) newcap = n;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = mem_resize(seq_t, cache, newcap);
#else
        tmp = realloc(cache, newcap * sizeof(seq_t));
#endif
        if (!tmp) {
            /* allocation failure – in the context of OEIS evaluation
               we can abort, but here we return 0 as a safe fallback. */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms up to n */
    for (idx_t i = used + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)4;
        } else if (i == 2) {
            cache[1] = (seq_t)9;
        } else if (i == 3) {
            cache[2] = (seq_t)25;
        } else {
            /* a(i) depends on a(i-1) and a(i-2) */
            seq_t prev  = cache[i - 2]; /* a(i-1)  */
            seq_t prev2 = cache[i - 3]; /* a(i-2)  */
            seq_t cand  = prev + (seq_t)1;

            while (1) {
                if (is_prime_big(cand)) { /* skip primes */
                    ++cand;
                    continue;
                }
                /* composite check omitted because is_prime_big returns false
                   for composite or 1, and cand > prev >= 4 here. */
                if (gcd(cand, prev) == (seq_t)1 && gcd(cand, prev2) == (seq_t)1) {
                    break;                /* suitable composite found */
                }
                ++cand;
            }
            cache[i - 1] = cand;
        }
        used = i;
    }

    return cache[n - 1];
}
