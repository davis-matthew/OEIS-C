/*
 * A084451  Juggling states associated with the juggling sequence A084452.
 * https://oeis.org/A084451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 1311, u32 n <= 9698, u64 n <= 9698,
 *            u128 n <= 9698, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084451 – Juggling states associated with the juggling sequence A084452
   Recurrence (from the OEIS entry):
        a(0) = 7
        a(n) = a(n-1)/2                     if a(n-1) is even
        a(n) = (a(n-1)-1)/2 + 2^e           otherwise,
           where e is the smallest non‑negative integer such that
             * a(n) has not appeared earlier, and
             * a(n) contains at least three 1‑bits in its binary representation. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* dynamic cache of the computed sequence */
static seq_t *cache = NULL;   /* array of terms */
static size_t cache_len = 0;  /* number of terms stored (>=1 after init) */
static size_t cache_cap = 0;  /* allocated capacity */

/* count number of 1-bits in x (Hamming weight) */
static inline int popcount_seq_t(seq_t x)
{
    int c = 0;
    while (x)
    {
        x &= x - (seq_t)1;
        c++;
    }
    return c;
}

/* ensure cache holds terms up to index `need` (inclusive) */
static void ensure_cache(idx_t need)
{
    /* initialise on first use */
    if (cache_len == 0)
    {
        cache_cap = 256;                     /* start with a modest capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)7;
        cache_len = 1;
    }

    size_t need_sz = (size_t)need;
    if (need_sz < cache_len) return;          /* already computed */

    /* grow capacity if necessary */
    if (need_sz + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        if (new_cap < need_sz + 1) new_cap = need_sz + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i <= need_sz; ++i)
    {
        seq_t prev = cache[i - 1];
        seq_t next;

        if ((prev & (seq_t)1) == 0)                   /* even case */
        {
            next = prev >> 1;
        }
        else                                        /* odd case */
        {
            seq_t base = (prev - (seq_t)1) >> 1;
            seq_t candidate;
            idx_t e = 0;

            while (1)
            {
                candidate = base + pow2((idx_t)e);

                /* check uniqueness */
                bool used = false;
                for (size_t k = 0; k < cache_len; ++k)
                {
                    if (cache[k] == candidate)
                    {
                        used = true;
                        break;
                    }
                }
                if (used) { ++e; continue; }

                /* check popcount >= 3 */
                if (popcount_seq_t(candidate) < 3) { ++e; continue; }

                /* candidate found */
                break;
            }
            next = candidate;
        }

        cache[i] = next;
        cache_len = i + 1;
    }
}

/* public function to obtain a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
