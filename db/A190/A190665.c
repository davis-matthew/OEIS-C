/*
 * A190665  Numbers k such that sum of aliquot parts of k is a nontrivial
 *          power: sigma(k) - k = a^b for integers a>1, b>1.
 * https://oeis.org/A190665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   big_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A190665
 *  a(n) = n-th positive integer k (offset 1) such that the sum of
 *         proper divisors of k, i.e. sigma(k)-k, is a non‑trivial
 *         perfect power a^b with integers a>1 and b>1. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n);   */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */

/* cache of found terms */
static seq_t *cache   = NULL;      /* allocated terms */
static size_t cache_len = 0;       /* number of stored terms  */
static size_t cache_cap = 0;       /* allocated capacity    */
static idx_t  next_k   = 1;        /* integer to test next */

/* ------------------------------------------------------------------ */
/* Test whether s is a perfect power a^b with a>1 and b>1               */
/* ------------------------------------------------------------------ */
static bool is_perfect_power(seq_t s)
{
    if (s <= (seq_t)1) return false;       /* need a>1, b>1 */

    /* exponent e starts at 2 and increases until 2^e > s */
    for (int e = 2; ; ++e)
    {
        seq_t exp = (seq_t)e;
        seq_t pow2 = big_pow((seq_t)2, exp);
        if (pow2 > s) break;               /* no larger exponent possible */

        /* Binary search for a base a such that a^e == s. */
        seq_t low  = 2;
        seq_t high = 2;

        /* Find high such that high^e > s */
        while (1)
        {
            seq_t ph = big_pow(high, exp);
            if (ph > s) break;
            high <<= 1;                    /* double high */
        }

        /* Binary search between low and high */
        while (low <= high)
        {
            seq_t mid = (low + high) >> 1;
            seq_t ph  = big_pow(mid, exp);
            if (ph == s) return true;      /* perfect power found */
            if (ph < s)   low  = mid + 1;
            else          high = mid - 1;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 */

    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* Test current integer next_k */
        seq_t sigma   = sigma_of(next_k);
        seq_t k_seq   = (seq_t)next_k;
        seq_t proper  = sigma - k_seq;      /* sigma(k) - k */

        if (is_perfect_power(proper))
        {
            /* Ensure sufficient capacity in the cache */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return 0;  /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k_seq;   /* store found term */
        }
        ++next_k;
    }
    return cache[n - 1];  /* terms are 1‑indexed */
}
