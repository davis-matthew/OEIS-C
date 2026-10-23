/*
 * A021149  Decimal expansion of 1/145.
 * https://oeis.org/A021149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021149: Decimal expansion of 1/145.
   a(0) = 0, a(1) = 0, a(2) = 6, a(3) = 8, …
   The n‑th digit after the decimal point of 1/145 is produced
   by the standard long‑division recurrence:
        r_0 = 1
        a(n) = (r_n * 10) / 145
        r_{n+1} = (r_n * 10) % 145
   where all divisions are integer divisions.                           */

#include <stdlib.h>

static seq_t *digits   = NULL;   /* cached digits */
static idx_t   dlen     = 0;      /* number of digits stored (next free index) */
static idx_t   dcap     = 0;      /* current allocated capacity */
static uint64_t rem      = 1;     /* remainder r_n (0 <= rem < 145) */

/* Ensure the digits array can hold at least [n] (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (dcap > n) return;
    idx_t new_cap = dcap ? dcap * 2 : 64;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)realloc(digits, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    digits = new_arr;
    dcap   = new_cap;
}

/* Compute digits from dlen up to and including n. */
static void
compute_until(idx_t n)
{
    if (dlen > n) return;
    ensure_capacity(n);
    for (idx_t i = dlen; i <= n; ++i) {
        seq_t digit = (seq_t)((rem * 10) / 145);
        rem        = (rem * 10) % 145;
        digits[i] = digit;
    }
    dlen = n + 1;
}

/* Public entry point: return the nth digit (zero‑based). */
seq_t a(idx_t n)
{
    if (!digits) {
        digits = (seq_t*)calloc(1, sizeof(seq_t));  /* one slot for a(0) */
        dcap   = 1;
        dlen   = 1;
        /* compute a(0) */
        seq_t digit = (seq_t)((rem * 10) / 145);
        rem        = (rem * 10) % 145;
        digits[0] = digit;
    }
    if (n >= dlen) compute_until(n);
    return digits[n];
}
