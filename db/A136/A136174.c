/*
 * A136174  Fibonacci numbers that contain all the digits 0 to 9 at least
 *          once.
 * https://oeis.org/A136174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 62, big any n
 * Helpers:   digit_counts, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth Fibonacci number that contains all decimal
 * digits 0–9 at least once. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/digit_counts.h"
#include "../utils/seq_realloc.h"

/* Grow an array of seq_t elements using the provided sequence reallocation
 * helper.  This guarantees proper construction of arbitrary–precision
 * objects. */
static void ensure_cap(seq_t **p, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 2 : (*cap << 1);
        while (newcap < needed) newcap <<= 1;

        if (*p == NULL) {
            *p = seq_realloc(*p, 0, newcap);
        } else {
            *p = seq_realloc(*p, *cap, newcap);
        }
        *cap = newcap;
    }
}

/* Static state for the generation of the sequence. */
static seq_t *complete   = NULL;   /* the collected Fibonacci numbers */
static size_t comp_len   = 0;       /* number collected so far   */
static size_t comp_cap   = 0;       /* allocated capacity        */

static seq_t  f_prev = (seq_t)0;    /* F(k-1) */
static seq_t  f_curr = (seq_t)1;    /* F(k)   */
static idx_t  idx     = 1;          /* current Fibonacci index   */

/* Append a Fibonacci number that contains all digits to the list. */
static void add_complete(seq_t value)
{
    ensure_cap(&complete, &comp_cap, comp_len + 1);
    complete[comp_len++] = value;
}

/* Generate Fibonacci numbers until we have found at least n of them. */
static void generate_until(size_t n)
{
    while (comp_len < n) {
        /* Check the current Fibonacci number. */
        unsigned char cnt[10];
        digit_counts(f_curr, cnt);

        bool all_digits = true;
        for (int d = 0; d < 10; ++d) {
            if (cnt[d] == 0) { all_digits = false; break; }
        }
        if (all_digits) {
            add_complete(f_curr);
        }

        /* Advance to next Fibonacci number. */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
        ++idx;
    }
}

/* Return the nth Fibonacci number that contains all digits 0–9. */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;   /* sequence is undefined for n <= 0 */

    generate_until((size_t)n);
    return complete[n - 1];
}
