/*
 * A128823  a(n) is the sum of all n-digit Fibonacci numbers.
 * https://oeis.org/A128823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of all Fibonacci numbers having exactly n decimal
 *            digits
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_len.h"

#ifdef __cplusplus
/* C++ implementation uses std::vector for convenient construction of arbitrary‑precision seq_t objects. */
#include <vector>

static std::vector<seq_t> a_cache;        /* a_cache[i] holds a(i) for i ≥ 1 */
static bool a_cache_valid = false;       /* whether a_cache has been initialized */
static idx_t a_max = 0;                  /* maximum n that has been computed so far */

static seq_t f_prev = 0;                 /* current Fibonacci number F(k)   */
static seq_t f_curr = 1;                 /* next Fibonacci number F(k+1)   */
static idx_t current_index = 1;          /* index of f_prev  (k)           */
static int cur_len = 0;                  /* decimal digit length of f_prev  */
static seq_t current_sum = 0;            /* running sum for cur_len         */

static void build_upto(idx_t n)
{
    if (n <= a_max) return;

    /* First call: allocate cache and initialize Fibonacci state */
    if (!a_cache_valid)
    {
        a_cache.resize(n + 1, 0);
        a_cache_valid = true;

        f_prev = (seq_t)1;          /* F1 */
        f_curr = (seq_t)1;          /* F2 */
        current_index = 1;
        cur_len = digit_len(f_prev);
        current_sum = 0;
    }
    else if (a_cache.size() <= static_cast<size_t>(n))
    {
        a_cache.resize(n + 1, 0);
    }

    /* Generate Fibonacci numbers until all digit lengths <= n have been processed */
    while (cur_len <= (int)n)
    {
        current_sum += f_prev;

        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        ++current_index;

        int next_len = digit_len(f_prev);
        if (next_len != cur_len)
        {
            a_cache[cur_len] = current_sum;   /* store sum for current_len */
            cur_len = next_len;
            current_sum = 0;
        }
    }
    a_max = n;
}

#else
/* C implementation: use malloc/realloc for seq_t assumed to be an integral type. */
static seq_t *a_cache = NULL;              /* a_cache[i] holds a(i) for i ≥ 1 */
static idx_t a_max = 0;                   /* maximum n that has been computed so far */

static seq_t f_prev = 0;                  /* current Fibonacci number F(k)   */
static seq_t f_curr = 1;                  /* next Fibonacci number F(k+1)   */
static idx_t current_index = 1;           /* index of f_prev  (k)           */
static int cur_len = 0;                   /* decimal digit length of f_prev  */
static seq_t current_sum = 0;             /* running sum for cur_len         */

static void build_upto(idx_t n)
{
    if (n <= a_max) return;

    if (a_cache == NULL)
    {
        a_cache = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (a_cache == NULL) exit(EXIT_FAILURE);
        /* Initialise Fibonacci state */
        f_prev = (seq_t)1;          /* F1 */
        f_curr = (seq_t)1;          /* F2 */
        current_index = 1;
        cur_len = digit_len(f_prev);
        current_sum = 0;
    }
    else if (n + 1 > a_max + 1)
    {
        seq_t *tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        /* Zero‑initialise newly allocated slots */
        for (idx_t i = a_max + 1; i <= n; ++i) tmp[i] = (seq_t)0;
        a_cache = tmp;
    }

    /* Generate Fibonacci numbers until all digit lengths <= n have been processed */
    while (cur_len <= (int)n)
    {
        current_sum += f_prev;

        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        ++current_index;

        int next_len = digit_len(f_prev);
        if (next_len != cur_len)
        {
            a_cache[cur_len] = current_sum;   /* store sum for current_len */
            cur_len = next_len;
            current_sum = 0;
        }
    }
    a_max = n;
}
#endif

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    build_upto(n);
#ifdef __cplusplus
    return a_cache[(size_t)n];
#else
    return a_cache[n];
#endif
}
