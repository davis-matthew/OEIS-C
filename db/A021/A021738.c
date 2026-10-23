/*
 * A021738  Decimal expansion of 1/734.
 * https://oeis.org/A021738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence gives the decimal expansion of 1/734 after the decimal point.
   a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 3, …  (see OEIS A021738).
   The value is produced by repeated long division:
     remainder *= 10
     digit      = remainder / 734
     remainder  = remainder % 734
*/

#include <stdlib.h>

static seq_t *digits = NULL;      /* cached digits */
static size_t digits_cap = 0;     /* allocated capacity */
static size_t digits_len = 0;     /* number of digits cached */
static uint64_t remainder = 1;    /* remainder after the last cached digit */

static void ensure_capacity(size_t needed)
{
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap * 2 : 64;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    if (!new_arr) abort();        /* out of memory – terminate */
    digits = new_arr;
    digits_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* If we already computed the digit, simply return it. */
    if (digits_len > (size_t)n) return digits[(size_t)n];

    /* Compute digits up to index n. */
    while (digits_len <= (size_t)n)
    {
        remainder *= 10ULL;                          /* 64‑bit intermediate */
        uint64_t q = remainder / 734ULL;             /* digit */
        remainder %= 734ULL;
        ensure_capacity(digits_len + 1);
        digits[digits_len++] = (seq_t)q;
    }
    return digits[(size_t)n];
}
