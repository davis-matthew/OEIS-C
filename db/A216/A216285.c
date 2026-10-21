/*
 * A216285  Primes which cannot be written as x^4+y^4.
 * https://oeis.org/A216285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 4859, u32 n <= 6498, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216285 – primes that cannot be written as x^4 + y^4.
   The function keeps a cache of found terms and extends it
   as necessary.  It works for seq_t = unsigned integer
   (8/16/32/64/128 bits) or an arbitrary‑precision class. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/grow_seq.h"       /* grow_seq(seq_t **, size_t*, size_t) */

static seq_t *terms    = NULL;
static size_t  terms_sz = 0;   /* number of stored terms       */
static size_t  terms_cap = 0;  /* allocated capacity           */
static uint64_t last_checked = 1;  /* last integer examined (>=1) */

/* integer fourth‑root for 64‑bit values, result fits in uint64_t */
static uint64_t fourth_root(uint64_t x)
{
    uint64_t lo = 0, hi = 65536ULL;           /* 65536^4 == 2^64 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __uint128_t m4 = (__uint128_t)mid * mid * mid * mid;
        if (m4 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* test whether a 64‑bit prime can be expressed as a^4 + b^4 (a,b >=1) */
static bool representable(uint64_t p)
{
    uint64_t a_max = fourth_root(p);
    for (uint64_t a = 1; a <= a_max; ++a) {
        __uint128_t a4 = (__uint128_t)a * a * a * a;
        if (a4 > p) break;
        uint64_t rem = p - (uint64_t)a4;
        uint64_t b = fourth_root(rem);
        if ((__uint128_t)b * b * b * b == rem)
            return true;          /* found a representation */
    }
    return false;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1, but guard */

    /* extend cache up to n terms */
    while (terms_sz < (size_t)n) {
        /* next candidate: odd integers starting from 3 */
        uint64_t cand = last_checked < 3 ? 3 : last_checked + 1;
        if (cand < 3) cand = 3;
        if ((cand & 1) == 0) cand++;   /* make it odd */

        /* test primality */
        if (is_prime_big((seq_t)cand)) {
            /* check representability as sum of two fourth powers */
            if (!representable((uint64_t)cand)) {
                /* store the term */
                if (terms_sz == terms_cap)
                    grow_seq(&terms, &terms_cap, terms_sz + 1);
                terms[terms_sz++] = (seq_t)cand;
            }
        }
        last_checked = cand;
    }

    return terms[(size_t)n - 1];
}
