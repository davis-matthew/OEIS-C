/*
 * A040116  Primes p such that x^4 = 9 has a solution mod p.
 * https://oeis.org/A040116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040116: primes p such that x^4 = 9 has a solution mod p */
/* Algorithm:
   p = 2   → include.
   p ≡ 3 (mod 4) → include (Legendre(9, p) = 1).
   p ≡ 1 (mod 4) → include iff Legendre(3, p) = 1,
                    i.e. 3^((p‑1)/2) ≡ 1 (mod p). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x) */
#include "../utils/seq_realloc.h"  /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

static seq_t *CACHE = NULL;  /* cache of terms */
static size_t CNT   = 0;     /* number of stored terms */
static size_t CAP   = 0;     /* cache capacity */
static uint64_t NEXT = 2;    /* next integer to test */

/* Ensure cache has at least 'need' capacity. */
static void grow_cache(size_t need)
{
    if (CAP < need) {
        size_t newcap = CAP ? CAP << 1 : 16;
        while (newcap < need) newcap <<= 1;
        CACHE = seq_realloc(CACHE, CAP, newcap);
        CAP = newcap;
    }
}

/* Append a new term to the cache. */
static void cache_append(seq_t val)
{
    grow_cache(CNT + 1);
    CACHE[CNT++] = val;
}

/* Core sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; sequence is defined for n ≥ 1 */

    /* generate terms until we have at least n of them */
    while (CNT < (size_t)n) {
        uint64_t p = NEXT;

        /* advance NEXT for the next loop iteration */
        if (NEXT == 2)
            NEXT = 3;       /* skip even numbers after 2 */
        else
            NEXT += 2;

        if (!is_prime(p))          /* skip non‑primes */
            continue;

        bool include = false;
        if (p == 2)                /* p = 2 always qualifies */
            include = true;
        else if (p % 4 == 3)       /* p ≡ 3 (mod 4) always qualifies */
            include = true;
        else {                     /* p ≡ 1 (mod 4) → check Legendre(3, p) */
            seq_t leg = mod_pow((seq_t)3, (p - 1) / 2, (seq_t)p);
            include = (leg == (seq_t)1);
        }

        if (include)
            cache_append((seq_t)p);
    }

    return CACHE[n - 1];  /* zero‑based indexing: a(1) = CACHE[0] */
}
