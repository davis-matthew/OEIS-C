/*
 * A070938  Harshad numbers which terminate in their digital sum.
 * https://oeis.org/A070938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 312, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070938 – Harshad numbers that terminate in their digital sum.
   The value a(n) is the n‑th positive integer that satisfies
   (1) n is divisible by the sum of its decimal digits, and
   (2) its decimal representation ends with this digit sum.  */

/* The function is implemented by brute‑force search and caches results
   across calls.  It works for all supported builds (8–128 bit unsigned,
   C++ big integers, etc.) without using any language features that are
   unavailable on those builds. */

#include <stddef.h>
#include <stdbool.h>

/* Utility headers provided by the OEIS framework */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/*---------------------------------------------------------------------*/
/* Helper: number of decimal digits of a strictly positive value.      */
static int digit_len(seq_t x)
{
    int len = 0;
    do {
        len++;
        x /= 10;
    } while (x);
    return len;
}

/* Helper: sum of decimal digits.  Assumes x ≥ 0.                     */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Helper: 10^k for small k (k ≤ 20 for 64‑bit, ≤39 for 128‑bit).     */
static seq_t pow10_digits(int k)
{
    seq_t p = 1;
    for (int i = 0; i < k; ++i)
        p *= 10;
    return p;
}

/*---------------------------------------------------------------------*/
/* Static cache of already found terms.  All operations are purely
   sequential and thread‑safe only if called from a single thread.     */
static seq_t *cache     = NULL;   /* array of results                */
static size_t cache_len = 0;      /* number of terms currently cached*/
static size_t cache_cap = 0;      /* allocated capacity (in terms)   */
static seq_t  next_candidate = 1; /* next integer to test            */

/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Initialise cache on first call.                                  */
    if (cache_cap == 0) {
        cache_cap = 64;                     /* initial capacity   */
        cache = alloc_seq_array(cache_cap); /* may return NULL on OOM */
    }

    /* Generate further terms until we have at least n of them.        */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;

        /* Find next candidate satisfying the conditions.             */
        while (true) {
            seq_t ds = digit_sum(cand);      /* digit sum          */
            if (ds == 0) {                   /* never happens for >0 */
                cand++;
                continue;
            }

            if ((cand % ds) == 0) {          /* Harshad            */
                int k   = digit_len(ds);     /* digits of sum      */
                seq_t p10 = pow10_digits(k); /* 10^k               */
                if ((cand % p10) == ds) {    /* ends with sum     */
                    /* Ensure capacity and store value.       */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap * 2;
                        seq_t *new_cache = alloc_seq_array(new_cap);
                        for (size_t i = 0; i < cache_cap; ++i)
                            new_cache[i] = cache[i];
                        free_seq_array(cache);
                        cache     = new_cache;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = cand;
                    next_candidate    = cand + 1;
                    break;                 /* ready for next term */
                }
            }
            cand++;                      /* try next integer     */
        }
    }

    return cache[(size_t)n - 1];
}
