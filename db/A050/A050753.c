/*
 * A050753  Sixth powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th sixth power with no consecutive equal digits
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 313, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 980 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* static state */
static seq_t *sixth_vals = NULL;
static size_t sixth_len = 0;
static size_t sixth_cap = 0;
static idx_t next_base = 0;

static void grow_buffer(void)
{
    size_t new_cap = sixth_cap ? sixth_cap * 2 : 16;
    seq_t *new_buf = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < sixth_len; ++i) {
        new_buf[i] = sixth_vals[i];
    }
    if (sixth_vals) {
        seq_free_array(sixth_vals);
    }
    sixth_vals = new_buf;
    sixth_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* generate until we have n+1 terms */
    while (sixth_len <= n) {
        seq_t base = (seq_t)next_base;
        seq_t sq  = base * base;      /* base^2 */
        seq_t q   = sq * sq;          /* base^4 */
        seq_t val = q * sq;           /* base^6 */

        if (no_consecutive_equal_digits(val)) {
            if (sixth_len == sixth_cap) {
                grow_buffer();
            }
            sixth_vals[sixth_len++] = val;
        }
        ++next_base;
    }
    return sixth_vals[n];
}
