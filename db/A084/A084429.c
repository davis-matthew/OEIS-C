/*
 * A084429  Integers m such that the base-10 digit concatenation
 *          4//m//3//m//2//m//1 is prime.
 * https://oeis.org/A084429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   alloc_seq_array, digit_len, isPrimeGeneric, pow10,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A084429: a(n) = the n‑th integer m such that the decimal
 * concatenation 4‖m‖3‖m‖2‖m‖1 is prime.  The function uses a simple
 * incremental search and caches found terms.  The implementation relies
 * on the helper functions provided by the build: digit_len, pow10,
 * isPrimeGeneric, alloc_seq_array and seq_free_array. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/digit_len.h"          /* digit_len(seq_t) -> int */
#include "../utils/pow10.h"              /* pow10(idx_t) -> seq_t */
#include "../utils/isPrimeGeneric.h"     /* isPrimeGeneric(seq_t) -> bool */
#include "../utils/alloc_seq_array.h"    /* alloc_seq_array(size_t) -> seq_t* */
#include "../utils/seq_free_array.h"    /* seq_free_array(seq_t*) */

/* Cache for already discovered terms. */
static seq_t *cache      = NULL;   /* array of previously found m values */
static size_t cache_len  = 0;      /* number of valid entries in cache   */
static size_t cache_cap  = 0;      /* current allocated capacity         */
static idx_t next_m      = 0;      /* next candidate to test            */

/* Ensure cache has the required capacity. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    seq_t *new_arr = alloc_seq_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];
    seq_free_array(cache);
    cache   = new_arr;
    cache_cap = new_cap;
}

/* Build the decimal number 4‖m‖3‖m‖2‖m‖1. */
static seq_t concat_4_3_2_1(idx_t m_val)
{
    seq_t m   = (seq_t)m_val;
    int   d   = digit_len(m);          /* number of decimal digits of m */
    seq_t p10 = pow10((idx_t)d);        /* 10^d */

    seq_t x = (seq_t)4;
    x = x * p10 + m;                    /* 4‖m */
    x = x * (seq_t)10 + (seq_t)3;        /* 4‖m‖3 */
    x = x * p10 + m;                    /* 4‖m‖3‖m */
    x = x * (seq_t)10 + (seq_t)2;        /* 4‖m‖3‖m‖2 */
    x = x * p10 + m;                    /* 4‖m‖3‖m‖2‖m */
    x = x * (seq_t)10 + (seq_t)1;        /* 4‖m‖3‖m‖2‖m‖1 */
    return x;
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache      = alloc_seq_array(128);
        cache_cap  = 128;
        cache_len  = 0;
        next_m     = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cand = concat_4_3_2_1(next_m);
        if (isPrimeGeneric(cand)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }

    return cache[n - 1];
}
