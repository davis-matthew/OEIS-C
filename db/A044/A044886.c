/*
 * A044886  Numbers having, in base 15, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search based implementation: a(n) is the nth positive integer whose
   base‑15 representation has equal totals of run lengths with even
   length and with odd length.  The function stores found terms in a
   static array so that calls with arbitrary n order are handled. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool has_equal_even_odd_run_sums(seq_t x)
{
    if (x == 0) return false;

    /* Maximum number of base‑15 digits that will be handled safely
       for the range of n requested (a conservative 1024). */
    uint8_t digits[1024];
    idx_t len = 0;

    /* Extract digits (least‑significant first). */
    while (x != 0) {
        seq_t d = x % 15;
        x = x / 15;
        if (len >= 1024) return false;      /* overflow guard */
        digits[len++] = (uint8_t)(d);
    }
    if (len == 0) return false;                /* should not happen */

    /* Scan runs from most‑significant to least‑significant. */
    idx_t sum_even = 0;
    idx_t sum_odd  = 0;

    idx_t run_len = 1;
    uint8_t prev = digits[len - 1];
    for (idx_t pos = len - 2; pos < len; --pos) {   /* pos will underflow to MAX when loop finishes */
        uint8_t cur = digits[pos];
        if (cur == prev) {
            ++run_len;
        } else {
            if (run_len & 1) sum_odd += run_len;
            else             sum_even += run_len;
            run_len = 1;
            prev = cur;
        }
    }
    /* Handle the last run. */
    if (run_len & 1) sum_odd += run_len;
    else             sum_even += run_len;

    return sum_even == sum_odd;
}

static seq_t *terms_list = NULL;   /* array of found terms */
static size_t terms_cap  = 0;      /* current capacity */
static idx_t  term_count  = 0;     /* number of terms found */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    /* Grow the list until we have at least n terms. */
    while (term_count < n) {
        seq_t cur = (term_count ? terms_list[term_count - 1] + 1 : 1);

        while (!has_equal_even_odd_run_sums(cur)) ++cur;

        /* Append cur to the terms list. */
        if (term_count == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 8;
            seq_t *tmp = mem_resize(seq_t, terms_list, new_cap);
            terms_list = tmp;
            terms_cap = new_cap;
        }
        terms_list[term_count++] = cur;
    }

    return terms_list[n - 1];
}
