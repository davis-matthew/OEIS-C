/*
 * A115700  Squares whose digit reversal is the product of 2 palindromes
 *          greater than 1.
 * https://oeis.org/A115700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   floor_sqrt, is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115700: Squares whose digit reversal is the product of two palindromes > 1.
   The function generates the sequence by testing successive squares and
   checking the condition on each square.  It memoises results so that any
   order of calls is efficient.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/reverse_decimal.h"   /* seq_t reverse_decimal(seq_t x); */
#include "../utils/floor_sqrt.h"       /* seq_t floor_sqrt(seq_t x);    */
#include "../utils/is_palindrome.h"    /* bool is_palindrome(seq_t x, idx_t base); */

/* Utility to grow a dynamic array. */
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap ? *cap << 1 : 16);
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Global state for palindromes and cached sequence values     */
/* ------------------------------------------------------------ */
static seq_t *pal_arr       = NULL;   /* palindromes > 1 */
static size_t pal_cap       = 0;
static size_t pal_cnt       = 0;
static seq_t pal_next       = 2;     /* next integer to test for palindrome */

static seq_t *cached_vals   = NULL;   /* memoised a(n) values (0‑based) */
static size_t cache_cap     = 0;
static size_t cache_len     = 0;
static seq_t next_root      = 1;      /* next integer whose square to test */

/* ------------------------------------------------------------ */
/*  Helper to ensure pal_arr contains all palindromes <= max   */
/* ------------------------------------------------------------ */
static void ensure_pal_up_to(seq_t max_val)
{
    while (pal_next <= max_val) {
        if (is_palindrome(pal_next, 10)) {
            grow_array(&pal_arr, &pal_cap, pal_cnt + 1);
            pal_arr[pal_cnt++] = pal_next;
        }
        pal_next++;
    }
}

/* ------------------------------------------------------------ */
/*  Test whether sq is a “good” square for A115700                */
/* ------------------------------------------------------------ */
static bool is_good_square(seq_t sq)
{
    seq_t rev = reverse_decimal(sq);
    if (rev < 4) return false;   /* smallest product of two palindromes >1 is 2*2=4 */

    seq_t sqrt_rev = floor_sqrt(rev);
    ensure_pal_up_to(sqrt_rev);

    for (size_t i = 0; i < pal_cnt && pal_arr[i] <= sqrt_rev; ++i) {
        seq_t p = pal_arr[i];
        if (rev % p != 0) continue;
        seq_t q = rev / p;
        if (q > 1 && is_palindrome(q, 10))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                   */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but not used by tests */

    /* Grow cache to hold at least n elements */
    while (cache_len < (size_t)n) {
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap ? cache_cap << 1 : 16);
            seq_t *tmp = (seq_t*)realloc(cached_vals, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cached_vals = tmp;
            cache_cap = new_cap;
        }
        seq_t sq = next_root * next_root;
        next_root++;
        if (is_good_square(sq)) {
            cached_vals[cache_len++] = sq;
        }
    }
    return cached_vals[n - 1];
}
