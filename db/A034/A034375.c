/*
 * A034375  Fibonacci numbers with all odd digits (probably finite).
 * https://oeis.org/A034375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   all_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth Fibonacci number whose decimal digits are all odd. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/all_odd.h"

/* static state */
static seq_t *odd_fibs = NULL;    /* array of odd‑digit Fibonacci numbers */
static size_t odd_cap = 0;        /* allocated capacity */
static size_t odd_len = 0;        /* number of terms stored */

static seq_t fp_prev = (seq_t)1;  /* F_{k-1} */
static seq_t fp_curr = (seq_t)1;  /* F_{k}   */
static bool init_done = false;   /* flag */

/* grow the array to at least min_cap elements */
static void grow(size_t min_cap)
{
    if (odd_cap >= min_cap) return;
    size_t new_cap = odd_cap ? odd_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(odd_fibs, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();
    odd_fibs = tmp;
    odd_cap = new_cap;
}

/* initialise static state */
static void init_state(void)
{
    if (init_done) return;
    grow(16);
    odd_len = 0;
    /* first two Fibonacci numbers: 1, 1 */
    if (odd_len < odd_cap) odd_fibs[odd_len++] = (seq_t)1;
    if (odd_len < odd_cap) odd_fibs[odd_len++] = (seq_t)1;
    fp_prev = (seq_t)1;
    fp_curr = (seq_t)1;
    init_done = true;
}

/* generate odd‑digit Fibonacci numbers until index n is present */
static void ensure(idx_t n)
{
    init_state();
    while (odd_len <= (size_t)n)
    {
        seq_t next = fp_prev + fp_curr;
        fp_prev = fp_curr;
        fp_curr = next;
        if (all_odd(next))
        {
            grow(odd_len + 1);
            odd_fibs[odd_len++] = next;
        }
    }
}

/* external interface required for the competition */
seq_t a(idx_t n)
{
    ensure(n);
    return odd_fibs[(size_t)n];
}
