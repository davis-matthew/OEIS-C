/*
 * A134732  Concatenation of first n members of the Padovan sequence A000931,
 *          starting at (1, 1, 1, 2).
 * https://oeis.org/A134732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the first n members of the Padovan
 *            sequence defined by P(1)=P(2)=P(3)=1, P(n)=P(n-2)+P(n-3) for
 *            n≥4. a(1)=1, a(2)=11, a(3)=111, ... etc
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 15, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 116 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static idx_t cached_n = 0;
static seq_t *pad = NULL;
static seq_t *concat = NULL;

static void ensure_capacity(idx_t n)
{
    if (n <= cached_n) return;

    /* Grow arrays to hold indices 1..n */
    size_t newlen = (size_t)n;
    if (!pad) {
        pad = mem_new(seq_t, newlen);
        concat = mem_new(seq_t, newlen);
    } else {
        pad = mem_resize(seq_t, pad, newlen);
        concat = mem_resize(seq_t, concat, newlen);
    }

    /* Compute Padovan numbers and cumulative concatenations */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        idx_t idx = i - 1;          /* zero‑based index in the arrays */

        /* Padovan numbers */
        if (i <= 3) {
            pad[idx] = seq_one();
        } else {
            /* pad[i-1] = pad[i-3] + pad[i-4] (zero‑based) */
            pad[idx] = pad[idx-2] + pad[idx-3];
        }

        /* Cumulative concatenation */
        if (i == 1) {
            concat[idx] = pad[idx];
        } else {
            idx_t d   = num_digits(pad[idx]);  /* number of decimal digits */
            seq_t p10 = pow10(d);             /* 10^d as seq_t */
            concat[idx] = concat[idx-1] * p10 + pad[idx];
        }
    }

    cached_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for n < 1 – return 0 */
        return 0;
    }
    ensure_capacity(n);
    return concat[n - 1];
}
