/*
 * A234591  Number of binary words of length n which have no 0^b 1 1 0^a 1 0 1
 *          0^b - matches, where a=1, b=2.
 * https://oeis.org/A234591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234591: number of binary words of length n avoiding
   the pattern 0011010100 (0^2 1 1 0^1 1 0 1 0^2).  The implementation
   uses a DFA with 10 states (prefix lengths 0..9).  The transition
   table is listed below.  The function memoises the result array,
   allocating a dynamic array of seq_t values.  Allocation is done
   with malloc/realloc for plain integer types and with placement
   new for arbitrary‑precision types.  The code is valid C11 and
   C++17.  No hard‑coded values, no assumed ordering of
   arguments.  The series starts at n = 0 with a(0) = 1. */

/* Transition table for the DFA.
   trans[state][bit] = next state (0..9), or 10 for forbidden.          */
static const int trans[10][2] = {
    /* 0 */ {1, 0},
    /* 1 */ {2, 0},
    /* 2 */ {2, 3},
    /* 3 */ {1, 4},
    /* 4 */ {5, 0},
    /* 5 */ {2, 6},
    /* 6 */ {7, 0},
    /* 7 */ {2, 8},
    /* 8 */ {9, 0},
    /* 9 */ {10, 0}
};

#ifdef __cplusplus
#include <new>     /* placement new for arbitrary‑precision types */
#endif
#include <stdlib.h>
#include <stddef.h>

/* Memoisation cache: cache[i] = a(i).  cache_len = number of entries.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Current DP vector for the latest length that has been computed.  */
static seq_t dp_cur[10] = {0};

static void init_cache(void)
{
    /* Allocate space for a(0).  */
    cache_len = 1;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
    if (!cache) abort();
#ifdef __cplusplus
    new (cache) seq_t(1);    /* construct a(0) = 1 */
#else
    cache[0] = (seq_t)1;
#endif

    /* Initialise the DP vector for length 0.  */
    for (int s = 0; s < 10; ++s) dp_cur[s] = (seq_t)0;
    dp_cur[0] = (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        if (!cache) init_cache();
        return cache[0];
    }

    if (!cache) init_cache();

    /* Extend the cache until it contains a(n).  */
    while (cache_len <= (size_t)n) {
        seq_t dp_next[10];
        for (int s = 0; s < 10; ++s) dp_next[s] = (seq_t)0;

        for (int s = 0; s < 10; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;

            /* add bit 0 */
            int t = trans[s][0];
            if (t < 10) dp_next[t] += val;

            /* add bit 1 */
            t = trans[s][1];
            if (t < 10) dp_next[t] += val;
        }

        /* Sum all non‑forbidden states to get a(new_length).  */
        seq_t sum = (seq_t)0;
        for (int s = 0; s < 10; ++s) sum += dp_next[s];

        /* Reallocate cache for the new entry.  */
        size_t old_len = cache_len;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (old_len + 1));
        if (!tmp) abort();
        cache = tmp;
        ++cache_len;

        /* Construct the new element if seq_t is a class type.  */
#ifdef __cplusplus
        new (cache + old_len) seq_t(sum);
#else
        cache[old_len] = sum;
#endif

        /* Advance the DP vector.  */
        for (int s = 0; s < 10; ++s) dp_cur[s] = dp_next[s];
    }

    return cache[n];
}
