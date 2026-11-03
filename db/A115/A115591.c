/*
 * A115591  Primes p such that the multiplicative order of 2 modulo p is
 *          (p-1)/2.
 * https://oeis.org/A115591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 958 (2026-11-03)
 * Helpers:   factor_u64, mem, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the multiplicative order of 2 modulo p is (p-1)/2 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* static caches for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t last_checked_prime = 3;   /* prime before the first candidate */
static idx_t   computed_up_to  = 0;       /* number of terms that have been generated */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for n < 1 */

    /* Return cached value if already computed */
    if (n <= computed_up_to)
        return cache[n-1];

    /* Generate terms until we reach n */
    while (computed_up_to < n) {
        uint64_t p;
        while (1) {
            p = next_prime(last_checked_prime);
            last_checked_prime = p;

            /* p must be 1 or 7 (mod 8) for 2 to be a quadratic residue */
            if ((p & 7) != 1 && (p & 7) != 7)
                continue;

            uint64_t e = (p - 1) / 2ULL;      /* exponent of 2 to test */

            /* Verify that 2^e ≡ 1 (mod p) */
            if (powmod((seq_t)2, e, (seq_t)p) != (seq_t)1)
                continue;

            /* Factor e and ensure no smaller divisor gives 1 */
            uint64_t pr[64], ex[64];
            size_t   cnt;
            factor_u64(e, pr, ex, &cnt);

            bool ok = true;
            for (size_t i = 0; ok && i < cnt; ++i) {
                uint64_t q   = pr[i];
                uint64_t exp = e / q;
                if (powmod((seq_t)2, exp, (seq_t)p) == (seq_t)1)
                    ok = false;
            }

            if (ok) {
                append_candidate(&cache, &cache_len, &cache_cap,
                                 (seq_t)p);
                break;                         /* term found */
            }
        }
        ++computed_up_to;
    }

    return cache[n-1];
}
